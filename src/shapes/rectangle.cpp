#include "rectangle.h"

using namespace std;
using namespace cv;

namespace canvascv
{

const char * Rectangle::type = "Rectangle";

Rectangle::Rectangle(const Point &pos)
{
    pt1 = addShape<Handle>(Point(pos.x-1, pos.y-1));
    pt2 = addShape<Handle>(Point(pos.x+1, pos.y-1));
    pt3 = addShape<Handle>(Point(pos.x+1, pos.y+1));
    pt4 = addShape<Handle>(Point(pos.x-1, pos.y+1));
    rotate1 = addShape<Handle>(Point(pos.x+1, pos.y));
    rotate2 = addShape<Handle>(Point(pos.x, pos.y+1));
    rotate3 = addShape<Handle>(Point(pos.x-1, pos.y));
    rotate4 = addShape<Handle>(Point(pos.x, pos.y-1));
    center = addShape<Handle>(pos);
    center->setVisible(false);
    width = 3;
    height = 3;
    angle = 0;
    selfUpdate = false;
    updatePoints();
    registerCBs();
    setActive(pt3);
}

void Rectangle::draw(Mat &canvas)
{
    line(canvas, (*pt1)(), (*pt2)(), outlineColor, thickness, lineType);
    line(canvas, (*pt2)(), (*pt3)(), outlineColor, thickness, lineType);
    line(canvas, (*pt3)(), (*pt4)(), outlineColor, thickness, lineType);
    line(canvas, (*pt4)(), (*pt1)(), outlineColor, thickness, lineType);
    CompoundShape::draw(canvas);
}

bool Rectangle::mousePressed(const Point &pos, bool onCreate)
{
    if (isReady())
    {
        if (CompoundShape::mousePressed(pos, onCreate))
        {
            // one of the points might have lost it's focus
            //  by the generic CompoundShape::mousePressed()
            for (auto &pt : {pt1, pt2, pt3, pt4, rotate1, rotate2, rotate3, rotate4})
            {
                pt->setVisible(true);
            }
            return true;
        }

        if (isPointInRectangle(pos))
        {
            for (auto &pt : {pt1, pt2, pt3, pt4, rotate1, rotate2, rotate3, rotate4})
            {
                pt->setVisible(true);
            }
            return true;
        }
    }
    else
    {
        if ( onCreate) return true;

        setReady();
        return true;
    }

    return false;
}

bool Rectangle::mouseMoved(const Point &pos)
{
    if (isReady())
    {
        if (CompoundShape::mouseMoved(pos))
        {
            return true;
        }

        if (isPointInRectangle(pos))
        {
            return true;
        }
    }
    else
    {
        pt3->setPos(pos);
        return true;
    }

    return false;
}

bool Rectangle::mouseReleased(const Point &pos)
{
    if (isReady())
    {
        if (CompoundShape::mouseReleased(pos))
        {
            return true;
        }

        if (isPointInRectangle(pos))
        {
            // mouse unclicked in rect
            for (auto &pt : {pt1, pt2, pt3, pt4, rotate1, rotate2, rotate3, rotate4})
            {
                pt->setVisible(true);
            }
            return true;
        }
    }
    else
    {
        return true;
    }

    return false;
}

void Rectangle::recalcRect(const Point &pos, bool rotated, float offset)
{
    if (rotated)
    {
        const Point &centerPoint = (*center)();
        angle = atan2(pos.y - centerPoint.y, pos.x - centerPoint.x);
        angle -= offset;
    }
    else
    {
        const Point &centerPoint = (*center)();
        float distance = sqrt(pow(pos.x-centerPoint.x,2)+pow(pos.y-centerPoint.y,2));
        float angleToPoint = atan2(pos.y - centerPoint.y, pos.x - centerPoint.x);
        angleToPoint -= angle;
        width = abs(cos(angleToPoint)*distance*2);
        height = abs(sin(angleToPoint)*distance*2);
    }

    updatePoints();
}

void Rectangle::updatePoints()
{
    float fSin = sin(angle)*0.5f;
    float fCos = cos(angle)*0.5f;
    const Point &centerPoint = (*center)();

    Point pt;

    pt.x = centerPoint.x + cos(angle) * width * 0.5f;
    pt.y = centerPoint.y + sin(angle) * width * 0.5f;
    rotate1->setPos(pt);

    pt.x = centerPoint.x + cos(angle+CV_PI/2) * height * 0.5f;
    pt.y = centerPoint.y + sin(angle+CV_PI/2) * height * 0.5f;
    rotate2->setPos(pt);

    pt.x = centerPoint.x + cos(angle+CV_PI) * width * 0.5f;
    pt.y = centerPoint.y + sin(angle+CV_PI) * width * 0.5f;
    rotate3->setPos(pt);

    pt.x = centerPoint.x + cos(angle+CV_PI*1.5) * height * 0.5f;
    pt.y = centerPoint.y + sin(angle+CV_PI*1.5) * height * 0.5f;
    rotate4->setPos(pt);

    pt.x = centerPoint.x - fSin*height - fCos*width;
    pt.y = centerPoint.y + fCos*height - fSin*width;
    pt1->setPos(pt);

    pt.x = centerPoint.x + fSin*height - fCos*width;
    pt.y = centerPoint.y - fCos*height - fSin*width;
    pt2->setPos(pt);

    pt.x = 2*centerPoint.x - (*pt1)().x;
    pt.y = 2*centerPoint.y - (*pt1)().y;
    pt3->setPos(pt);

    pt.x = 2*centerPoint.x - (*pt2)().x;
    pt.y = 2*centerPoint.y - (*pt2)().y;
    pt4->setPos(pt);
}

void Rectangle::registerCBs()
{
    for (auto &pt : {pt1, pt2, pt3, pt4})
    {
        pt->addPosChangedCB([this](const Point &pos)
        {
            if (!selfUpdate)
            {
                selfUpdate = true;
                recalcRect(pos);
                selfUpdate = false;
            }
        });
    }
    rotate1->addPosChangedCB([this](const Point &pos)
    {
        if (!selfUpdate)
        {
            selfUpdate = true;
            recalcRect(pos, true, 0);
            selfUpdate = false;
        }
    });
    rotate2->addPosChangedCB([this](const Point &pos)
    {
        if (!selfUpdate)
        {
            selfUpdate = true;
            recalcRect(pos, true, CV_PI/2);
            selfUpdate = false;
        }
    });
    rotate3->addPosChangedCB([this](const Point &pos)
    {
        if (!selfUpdate)
        {
            selfUpdate = true;
            recalcRect(pos, true, CV_PI);
            selfUpdate = false;
        }
    });
    rotate4->addPosChangedCB([this](const Point &pos)
    {
        if (!selfUpdate)
        {
            selfUpdate = true;
            recalcRect(pos, true, CV_PI*1.5);
            selfUpdate = false;
        }
    });
    center->addPosChangedCB([this](const Point)
    {
        if (!selfUpdate)
        {
            selfUpdate = true;
            updatePoints();
            selfUpdate = false;
        }
    });
}

void Rectangle::writeInternals(FileStorage &fs) const
{
    CompoundShape::writeInternals(fs);
    fs << "width" << width;
    fs << "height" << height;
    fs << "angle" << angle;
}

void Rectangle::readInternals(const FileNode &node)
{
    CompoundShape::readInternals(node);
    node["width"] >> width;
    node["height"] >> height;
    node["angle"] >> angle;
}

void Rectangle::reloadPointers(const list<Shape *> &lst, list<Shape*>::const_iterator &i)
{
    CompoundShape::reloadPointers(lst, i);
    pt1 = dynamic_cast<Handle*>(*i++);
    pt2 = dynamic_cast<Handle*>(*i++);
    pt3 = dynamic_cast<Handle*>(*i++);
    pt4 = dynamic_cast<Handle*>(*i++);
    rotate1 = dynamic_cast<Handle*>(*i++);
    rotate2 = dynamic_cast<Handle*>(*i++);
    rotate3 = dynamic_cast<Handle*>(*i++);
    rotate4 = dynamic_cast<Handle*>(*i++);
    center = dynamic_cast<Handle*>(*i++);
    registerCBs();
}

list<Handle *> Rectangle::getConnectionTargets()
{
    return {rotate1, rotate2, rotate3, rotate4};
}

const char *Rectangle::getType() const {
    return type;
}

RotatedRect Rectangle::getRect() const
{
    return RotatedRect((*center)(), Size(width, height), angle * 180 / CV_PI);
}

void Rectangle::setRect(const RotatedRect &rect)
{
    center->setPos(rect.center);
    width = rect.size.width;
    height = rect.size.height;
    angle = rect.angle * CV_PI / 180;
    updatePoints();
}

void Rectangle::translate(const Point &offset)
{
    Shape *shape = getActive();
    if (shape)
    {
        shape->translate(offset);
    }
    else
    {
        center->translate(offset);
    }
}

}
