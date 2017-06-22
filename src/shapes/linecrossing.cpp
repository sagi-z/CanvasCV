#include "linecrossing.h"
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

namespace canvascv {

const char * LineCrossing::type = "LineCrossing";

LineCrossing::LineCrossing(const Point &pos)
    : direction(-1), arrowMagnitude(60)
{
    line = addShape<Line>(pos);
    arrow = addShape<Arrow>(pos);
    arrow->setVisible(false);
    arrow->setLocked(true);
    textBox = addShape<TextBox>(pos);
    textBox->setVisible(false);
    textBox->setLocked(false);
    textBox->setText(type);
    registerCBs();
    setActive(line);
}

void LineCrossing::draw(Mat &canvas)
{
    if (line->length()<10)
    {
        arrow->setVisible(false);
        textBox->setVisible(false);
    } else {
        arrow->setVisible(true);
        textBox->setVisible(true);
        arrow->getPT1().setVisible(false);
        arrow->getPT2().setVisible(false);
    }
    CompoundShape::draw(canvas);
}

bool LineCrossing::mousePressed(const Point &pos, bool onCreate)
{
    if (isReady())
    {
        if (arrow->getVisible())
        {
            if (arrow->isPointOnLine(pos))
            {
                direction *= -1;
                recalcArrow();
                return true;
            }
        }

        return CompoundShape::mousePressed(pos, onCreate);
    }
    else
    {
        if (CompoundShape::mousePressed(pos, onCreate))
        {
            if (line->isReady()) setReady();
            return true;
        }
    }
    return false;
}

void LineCrossing::registerCBs()
{
    line->getPT1().addPosChangedCB([this](const Point &)
    {
        recalcArrow();
        recalcTextBox();
    });
    line->getPT2().addPosChangedCB([this](const Point &)
    {
        recalcArrow();
        recalcTextBox();
    });
}

void LineCrossing::recalcArrow()
{
    Point head, tail;
    arrowTailHead(tail, head);
    arrow->setTailPos(tail);
    arrow->setHeadPos(head);
}

void LineCrossing::recalcTextBox()
{
    textBox->setTL(middlePoint());
}

Point LineCrossing::middlePoint()
{
    const Point &p1 = line->getTail();
    const Point &p2 = line->getHead();
    return p1+(p2-p1)/2;
}

bool LineCrossing::isMiddlePoint(const Point &pos, int threshold)
{
    Point middle = middlePoint();
    return abs(pos.x - middle.x) <= threshold &&
            abs(pos.y - middle.y) <= threshold;
}

void LineCrossing::arrowTailHead(Point &tail, Point &head)
{
    double len = line->length()/2.;
    if (len > 0)
    {
        head = middlePoint();
        const Point &p2 = line->getHead();
        // calculate the (x,y) of the left side normal unit vector
        double x = -direction*(p2.y-head.y)/len;
        double y = direction*(p2.x-head.x)/len;
        head.x=head.x+x*10;
        head.y=head.y+y*10;
        tail.x=head.x+x*arrowMagnitude;
        tail.y=head.y+y*arrowMagnitude;
    }
}

void LineCrossing::writeInternals(FileStorage &fs) const
{
    CompoundShape::writeInternals(fs);
    fs << "direction" << direction;
}

void LineCrossing::readInternals(const FileNode &node)
{
    CompoundShape::readInternals(node);
    node["direction"] >> direction;
    registerCBs();
}

void LineCrossing::reloadPointers(const list<Shape *> &lst, list<Shape*>::const_iterator &i)
{
    CompoundShape::reloadPointers(lst, i);
    line = dynamic_cast<Line*>(*i++);
    arrow = dynamic_cast<Arrow*>(*i++);
    textBox = dynamic_cast<TextBox*>(*i++);
}

const string &LineCrossing::getEditStatusMsg() const
{
    const static string msg = "Click on the arrow to change crossing direction.\n"
                              "Click on the TextBox to edit it.\n"
                              "Drag line or Handles to position.";
    return msg;
}

int LineCrossing::cross_z(const Point &pt) const
{
    return cross_z(line->getTail(), line->getHead(), direction, pt);
}

int LineCrossing::cross_z(const Point &lineStart, const Point &lineEnd, int direction, const Point &pt) const
{
    // a and b are 2 vectors on the same plane with z = 0
    Point a(lineEnd.x-lineStart.x, lineEnd.y-lineStart.y);
    Point b(pt.x-lineStart.x, pt.y-lineStart.y);

    // their cross product vector has a z dimension, which is the only thing we want here
    int c_z = a.x*b.y - a.y*b.x;
    return c_z * direction;
}


list<Handle *> LineCrossing::getConnectionTargets()
{
    return textBox->getConnectionTargets();
}

const char *LineCrossing::getType() const
{
    return type;
}

int LineCrossing::getDirection() const
{
    return direction;
}

const string &LineCrossing::getName() const
{
    return textBox->getText();
}

void LineCrossing::setName(const string &value) const
{
    textBox->setText(value);
}

int LineCrossing::getArrowMagnitude() const
{
    return arrowMagnitude;
}

void LineCrossing::setArrowMagnitude(int mag)
{
    arrowMagnitude = mag;
}

const Point &LineCrossing::getTail() const
{
    return line->getTail();
}

const Point &LineCrossing::getHead() const
{
    return line->getHead();
}

TextBox *LineCrossing::getTextBox()
{
    return textBox;
}

Line *LineCrossing::getLine()
{
    return line;
}

Arrow *LineCrossing::getArrow()
{
    return arrow;
}

bool LineCrossing::wasCrossed(const Point &pt) const
{
    return cross_z(pt) < 0;
}

int LineCrossing::isCrossedBySegment(const Point &lineStart, const Point &lineEnd) const
{
    int z1 = cross_z(lineStart);
    int z2 = cross_z(lineEnd);
    if (z1 >= 0 && z2 <= 0)
    {   // Maybe crossing in the direction of the arrow
        int z3 = cross_z(lineStart, lineEnd, 1, line->getTail());
        int z4 = cross_z(lineStart, lineEnd, 1, line->getHead());
        if ( (z3 <= 0 && z4 >= 0)
             ||
             (z4 <= 0 && z3 >= 0) )
        {
            return 1;
        }
    }
    else if (z2 >= 0 && z1 <= 0)
    {   // Maybe crossing against the direction of the arrow
        int z3 = cross_z(lineStart, lineEnd, 1, line->getTail());
        int z4 = cross_z(lineStart, lineEnd, 1, line->getHead());
        if ( (z3 <= 0 && z4 >= 0)
             ||
             (z4 <= 0 && z3 >= 0) )
        {
            return -1;
        }
    }
    return 0;
}

}
