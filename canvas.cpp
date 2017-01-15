#include "canvas.h"
#include "colors.h"
#include "shapefactory.h"
#include "shapesconnector.h"
#include <algorithm>

namespace canvasvc
{

Canvas::Canvas()
{
}

Canvas::~Canvas()
{
    clear();
}

void Canvas::redrawOn(const cv::Mat &src, cv::Mat &dst)
{
    if (&src != &dst)
    {
        dst = src.clone();
    }
    for (auto &shape : shapes)
    {
        shape->draw(dst);
    }
}

void Canvas::onMousePress(const cv::Point &pos)
{
    // delegate to active
    if (active.get())
    {
        if (! active->mousePressed(pos))
        {
            active->lostFocus();
            broadcastModify(active.get());
            active.reset();
        }
        return;
    }

    // try to set active
    for (auto &shape : shapes)
    {
        if (shape->mousePressed(pos))
        {
            active = shape;
            return;
        }
    }

    // create a new shape
    shapes.push_back(std::shared_ptr<Shape>(ShapeFactory::newShape(shapeType,pos)));
    active = shapes.back();
    active->setCanvas(*this);
    broadcastCreate(active.get());
    if (! active->mousePressed(pos, true))
    {
        active->lostFocus();
        active.reset();
    }
}

void Canvas::onMouseRelease(const cv::Point &pos)
{
    if (active.get())
    {
        if (! active->mouseReleased(pos))
        {
            active->lostFocus();
            broadcastModify(active.get());
            active.reset();
        }
    }
}

void Canvas::onMouseMove(const cv::Point &pos)
{
    if (active.get())
    {
        active->mouseMoved(pos);
    }
}

void Canvas::consumeKey(int &key)
{
    if (key != -1)
    {
        if (active.get())
        {
            if (! active->keyPressed(key))
            {
                active->lostFocus();
                active.reset();
            }
        }
    }
}

void Canvas::deleteActive()
{
    if (active.get())
    {
        shapes.erase(find(shapes.begin(),shapes.end(),active));
        active->lostFocus();
        broadcastDelete(active.get());
        std::list<std::shared_ptr<ShapesConnector>> connectors;
        getShapes(connectors);
        for (auto &connector : connectors)
        {
            connector->disconnectShape(active->getId());
        }
        active.reset();
    }
}

void Canvas::notifyOnCreate(Canvas::CBType cb)
{
    createNotifs.push_back(cb);
}

void Canvas::notifyOnModify(Canvas::CBType cb)
{
    modifyNotifs.push_back(cb);
}

void Canvas::notifyOnDelete(Canvas::CBType cb)
{
    deleteNotifs.push_back(cb);
}

void Canvas::clear()
{
    active.reset();
    for (auto &shape : shapes)
    {
        broadcastDelete(shape.get());
    }
    shapes.clear();
}

std::shared_ptr<Shape> Canvas::getShape(int id)
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

void Canvas::getShapes(const Point &pos, std::list<std::shared_ptr<Shape> > &result)
{
    result.clear();
    for (auto &shape : shapes)
    {
        if (shape->isAtPos(pos))
        {
            result.push_back(shape);
        }
    }
}

void Canvas::broadcastCreate(Shape *shape)
{
    for (auto &cb : createNotifs)
    {
        cb(shape);
    }
}

void Canvas::broadcastModify(Shape *shape)
{
    for (auto &cb : modifyNotifs)
    {
        cb(shape);
    }
}

void Canvas::broadcastDelete(Shape *shape)
{
    for (auto &cb : deleteNotifs)
    {
        cb(shape);
    }
}

void write(cv::FileStorage& fs, const std::string&, const Canvas& x)
{
    if (x.active.get())
    {
    }
    fs << "{";
    fs << "shapes" << "[";
    for (auto &shape : x.shapes)
    {
        fs << *shape;
    }
    fs << "]";
    fs << "}";
    if (x.active.get()) {
    }
}

void read(const cv::FileNode& node, Canvas& x, const Canvas&)
{
    x.clear();
    cv::FileNode n = node["shapes"];
    FileNodeIterator it = n.begin(), it_end = n.end();
    for (; it != it_end; )
    { // ++it is done automatically by "it >> shape"
        Shape *shape = 0;
        it >> shape;
        assert(shape != 0);
        x.shapes.push_back(std::shared_ptr<Shape>(shape));
        shape->lostFocus();
        shape->setCanvas(x);
    }
    std::list<std::shared_ptr<ShapesConnector>> connectors;
    x.getShapes(connectors);
    for (auto &connector : connectors)
    {
        connector->reconnect();
    }
    for (auto &shape : x.shapes)
    {
        x.broadcastCreate(shape.get());
    }
}

}
