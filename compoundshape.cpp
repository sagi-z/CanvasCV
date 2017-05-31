#include "colors.h"
#include "compoundshape.h"

#include <algorithm>

namespace canvascv
{

void CompoundShape::draw(cv::Mat &canvas)
{
    if (visible)
    {
        for (auto &shape : shapes)
        {
            if (shape->getVisible())
            {
                shape->draw(canvas);
            }
        }
    }
}

bool CompoundShape::mousePressed(const cv::Point &pos, bool onCreate)
{
    if (visible)
    {
        // delegate to active
        if (active.get())
        {
            if (! active->mousePressed(pos, onCreate))
            {
                active->lostFocus();
                active.reset();
                return false;
            } else {
                return true;
            }
        }

        // try to set active
        for (auto &shape : shapes)
        {
            if (shape->mousePressed(pos))
            {
                active = shape;
                return true;
            }
        }
    }
    return false;
}

bool CompoundShape::mouseMoved(const cv::Point &pos)
{
    if (visible)
    {
        if (active.get())
        {
            return active->mouseMoved(pos);
        }
    }
    return false;
}

bool CompoundShape::mouseReleased(const cv::Point &pos)
{
    if (visible)
    {
        if (active.get())
        {
            if (! active->mouseReleased(pos))
            {
                active->lostFocus();
                active.reset();
                return false;
            }
            return true;
        }
    }
    return false;
}

void CompoundShape::setOutlineColor(const cv::Scalar &value)
{
    Shape::setOutlineColor(value);
    for (auto &shape : shapes)
    {
        if (std::strncmp("Handle", shape->getType(),6))
        {   // This is not a "Handle"
            shape->setOutlineColor(value);
        }
    }
}

void CompoundShape::setFillColor(const cv::Scalar &value)
{
    Shape::setFillColor(value);
    for (auto &shape : shapes)
    {
        if (std::strncmp("Handle", shape->getType(),6))
        {   // This is not a "Handle"
            shape->setFillColor(value);
        }
    }
}

void CompoundShape::setThickness(int value)
{
    Shape::setThickness(value);
    for (auto &shape : shapes)
    {
        shape->setThickness(value);
    }
}

void CompoundShape::setLineType(int value)
{
    Shape::setLineType(value);
    for (auto &shape : shapes)
    {
        shape->setLineType(value);
    }
}

void CompoundShape::setLocked(bool value)
{
    Shape::setLocked(value);
    for (auto &shape : shapes)
    {
        shape->setLocked(value);
    }
}

void CompoundShape::setVisible(bool value)
{
    Shape::setVisible(value);
    for (auto &shape : shapes)
    {
        shape->setVisible(value);
    }
}

std::shared_ptr<Shape> CompoundShape::getShape(int id)
{
    for (auto &shape : shapes)
    {
        if (shape->getId() == id)
        {
            return shape;
        }
        std::shared_ptr<Shape> subShape = shape->getShape(id);
        if (subShape.get())
        {
            return subShape;
        }
    }
    return nullptr;
}

bool CompoundShape::rmvShape(std::shared_ptr<Shape> &shape)
{
    std::list<std::shared_ptr<Shape>>::iterator i = find(shapes.begin(),shapes.end(),shape);
    if (i != shapes.end())
    {
        shapes.erase(i);
        return true;
    }
    return false;
}

void CompoundShape::delActiveShape()
{
    if (active.get())
    {
        rmvShape(active);
        active->lostFocus();
        active.reset();
    }
}

void CompoundShape::writeInternals(cv::FileStorage &fs) const
{
    Shape::writeInternals(fs);
    fs << "shapes" << "[";
    for (auto &shape : shapes)
    {
        fs << *shape;
    }
    fs << "]";
}

void CompoundShape::readInternals(const cv::FileNode &node)
{
    Shape::readInternals(node);
    active.reset();
    shapes.clear();
    cv::FileNode n = node["shapes"];
    FileNodeIterator it = n.begin(), it_end = n.end();
    std::list<Shape*> shapesTmp;
    for (; it != it_end; )
    { // ++it is done automatically by "it >> shape"
        Shape *shape = 0;
        it >> shape;
        assert(shape != 0);
        shapes.push_back(std::shared_ptr<Shape>(shape));
        shapesTmp.push_back(shape);
    }
    std::list<Shape*>::const_iterator i = shapesTmp.begin();
    reloadPointers(i);
}

bool CompoundShape::keyPressed(int &key)
{
    if (active.get())
    {
        if (! active->keyPressed(key))
        {
            active->lostFocus();
            active.reset();
            return false;
        }
    }
    return true;
}

void CompoundShape::lostFocus()
{
    for (auto &shape : shapes)
    {
        shape->lostFocus();
    }
}

}
