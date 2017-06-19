#include "line.h"
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

namespace canvascv
{

const char * Line::type = "Line";

Line::Line(const Point &pos)
{
    pt1 = addShape<Handle>(pos);
    pt1->setLocked(true);
    pt1->setVisible(false);
    pt2 = addShape<Handle>(pos);
}

void Line::draw(Mat &canvas)
{
    line(canvas,(*pt1)(), (*pt2)(), outlineColor, thickness, lineType);
    CompoundShape::draw(canvas);
}

bool Line::mousePressed(const Point &pos, bool onCreate)
{
    if (isReady())
    {
        if (CompoundShape::mousePressed(pos, onCreate))
        {
            return true;
        }

        if (isPointOnLine(pos))
        {
            if (! pt1->getLocked())
            {
                pt1->setVisible(true);
            }
            if (! pt2->getLocked())
            {
                pt2->setVisible(true);
            }
            return true;
        }
    }
    else
    {
        if ( onCreate) return true;

        setReady();
        pt1->setLocked(false);
        pt1->setVisible(true);
        return true;
    }

    return false;
}

bool Line::mouseMoved(const Point &pos)
{
    if (isReady())
    {
        if (CompoundShape::mouseMoved(pos))
        {
            return true;
        }

        if (isPointOnLine(pos))
        {
            return true;
        }
    }
    else
    {
       pt2->setPos(pos);
       return true;
    }

    return false;
}

bool Line::mouseReleased(const Point &pos)
{
    if (isReady())
    {
        if (CompoundShape::mouseReleased(pos))
        {
            return true;
        }

        if (isPointOnLine(pos))
        {
            // mouse unclicked on line
            return true;
        }
    }
    else
    {
        return true;
    }

    return false;
}

void Line::reloadPointers(const list<Shape*> &lst, list<Shape*>::const_iterator &i)
{
    CompoundShape::reloadPointers(lst, i);
    pt1 = dynamic_cast<Handle*>(*i++);
    pt2 = dynamic_cast<Handle*>(*i++);
    setReady();
}

list<Handle *> Line::getConnectionTargets()
{
    return {pt1, pt2};
}

void Line::lockTail(bool isLocked)
{
    pt1->setLocked(isLocked);
}

void Line::lockHead(bool isLocked)
{
    pt2->setLocked(isLocked);
}

void Line::showTail(bool isVisible)
{
    pt1->setVisible(isVisible);
}

void Line::showHead(bool isVisible)
{
    pt2->setVisible(isVisible);
}

void Line::setTailPos(const Point &pos)
{
    pt1->setPos(pos);
}

void Line::setHeadPos(const Point &pos)
{
    pt2->setPos(pos);
}

Handle &Line::getPT1() {
    return *pt1;
}

Handle &Line::getPT2() {
    return *pt2;
}

const Point &Line::getTail() const
{
    return (*pt1)();
}

const Point &Line::getHead() const
{
    return (*pt2)();
}

bool Line::keyPressed(int &key)
{
    if (isReady()) return true;

    if (key == 27)
    { // ESC
        key = -1; // consume key
        return false; // lostFocus() => the canvas will delete us
    }
    return true;
}

}
