#ifndef LINE_H
#define LINE_H

#include "compoundshape.h"
#include "shapefactory.h"
#include "handle.h"

#include <opencv2/highgui.hpp>

namespace canvasvc
{

class Line : public CompoundShape
{
public:
    Line(const cv::Point &pos)
    {
        pt1 = addShape<Handle>(pos);
        pt2 = addShape<Handle>(pos);
        dragDisabled = false;
    }

    virtual void draw(cv::Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false);
    virtual bool mouseMoved(const cv::Point &pos);
    virtual bool mouseReleased(const cv::Point &pos);
    virtual bool isAtPos(const cv::Point &pos)
    {
        return isPointOnLine(pos);
    }

    virtual std::list<Handle *> getConnectionTargets();
    virtual const char *getType() const {
        return "Line";
    }

    void lockTail(bool isLocked)
    {
        pt1->setLocked(isLocked);
    }

    void lockHead(bool isLocked)
    {
        pt2->setLocked(isLocked);
    }

    void showTail(bool isVisible)
    {
        pt1->setVisible(isVisible);
    }

    void showHead(bool isVisible)
    {
        pt2->setVisible(isVisible);
    }

    void setTailPos(const cv::Point& pos)
    {
        pt1->setPos(pos);
    }

    void setHeadPos(const cv::Point& pos)
    {
        pt2->setPos(pos);
    }

    int length() const {
        const cv::Point &p1 = (*pt1)();
        const cv::Point &p2 = (*pt2)();
        return sqrt(pow(p2.y-p1.y,2)+pow(p2.x-p1.x,2));
    }

    bool isPointOnLine(const cv::Point &p3, int threshold=3) const
    {
        threshold += thickness/2;
        const cv::Point &p1 = (*pt1)();
        const cv::Point &p2 = (*pt2)();
        double len = length();

        // Make sure it is roughly on the line
        if (threshold <= abs((p2.y-p1.y)*p3.x-(p2.x-p1.x)*p3.y+p2.x*p1.y-p2.y*p1.x) / len)
        {
            return false;
        }

        // For p3 to be in p1<->p2 range, distance from p3 to either p1 or p2
        //  cannot be more than distance from p1 to p2 (len).

        if(sqrt(pow(p3.y-p1.y,2)+pow(p3.x-p1.x,2)) > len)
        {
            return false;
        }

        if(sqrt(pow(p3.y-p2.y,2)+pow(p3.x-p2.x,2)) > len)
        {
            return false;
        }

        return true;
    }

    Handle &getPT1() {
        return *pt1;
    }

    Handle &getPT2() {
        return *pt2;
    }

    const cv::Point &getTail() const
    {
        return (*pt1)();
    }

    const cv::Point &getHead() const
    {
        return (*pt2)();
    }

protected:
    Handle* pt1;
    Handle* pt2;
    cv::Point dragPos;
    bool dragDisabled;

    virtual void reloadPointers(std::list<Shape*>::const_iterator &i)
    {
        CompoundShape::reloadPointers(i);
        pt1 = dynamic_cast<Handle*>(*i++);
        pt2 = dynamic_cast<Handle*>(*i++);
    }
};

}

#endif // LINE_H
