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
    pt2 = addShape<Handle>(pos);
    dragDisabled = false;
}

void Line::draw(Mat &canvas)
{
    line(canvas,(*pt1)(), (*pt2)(), outlineColor, thickness, lineType);
    CompoundShape::draw(canvas);
}

bool Line::mousePressed(const Point &pos, bool onCreate)
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
        if (! onCreate && ! locked && ! dragDisabled)
        {
            dragPos = pos;
        }
        return true;
    }

    return false;
}

bool Line::mouseMoved(const Point &pos)
{
    if (dragPos.x || dragPos.y)
    {
        Point diff = dragPos - pos;
        pt1->setPos((*pt1)() - diff);
        pt2->setPos((*pt2)() - diff);
        dragPos = pos;
        return true;
    }

    if (CompoundShape::mouseMoved(pos))
    {
        return true;
    }

    if (isPointOnLine(pos))
    {
        return true;
    }

    return false;
}

bool Line::mouseReleased(const Point &pos)
{
    if (dragPos.x || dragPos.y)
    {
        dragPos.x = 0;
        dragPos.y = 0;
        return true;
    }

    if (CompoundShape::mouseReleased(pos))
    {
        return true;
    }

    if (isPointOnLine(pos))
    {
        // mouse unclicked on line
        return true;
    }

    return false;
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

}
