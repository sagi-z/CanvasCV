#ifndef LINE_H
#define LINE_H

#include "compoundshape.h"
#include "shapefactory.h"
#include "handle.h"

#include <opencv2/highgui.hpp>

namespace canvascv
{

/**
 * @brief The Line class
 * Allows you to draw a line by mouse or from code
 */
class Line : public CompoundShape
{
public:
    /// set if to lock the tail (Handle) of the line
    void lockTail(bool isLocked);

    /// set if to lock the head (Handle) of the line
    void lockHead(bool isLocked);

    /// set if to show the tail (Handle) of the line
    void showTail(bool isVisible);

    /// set if to show the head (Handle) of the line
    void showHead(bool isVisible);

    /// will move the tail (Handle) of the line to pos
    void setTailPos(const cv::Point& pos);

    /// will move the head (Handle) of the line to pos
    void setHeadPos(const cv::Point& pos);

    /// returns the length of the line in pixels
    double length() const {
        const cv::Point &p1 = (*pt1)();
        const cv::Point &p2 = (*pt2)();
        return sqrt(pow(p2.y-p1.y,2)+pow(p2.x-p1.x,2));
    }

    /// returns true if the point 'p3' is on the line, give or take 'threshold' pixels
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

    /// get the Handle to the tail of the
    Handle &getPT1();

    /// get the Handle to the head of the
    Handle &getPT2();

    /// get the Point position og the tail Handle
    const cv::Point &getTail() const;

    /// get the Point position og the head Handle
    const cv::Point &getHead() const;

    virtual bool isAtPos(const cv::Point &pos)
    {
        return isPointOnLine(pos);
    }

    virtual bool keyPressed(int &key);

    virtual std::list<Handle *> getConnectionTargets();

    virtual const char *getType() const;

    static const char * type;

protected:
    friend class ShapeFactory;
    template <class T> friend class ShapeFactoryT;

    Line(const cv::Point &pos);

    virtual void draw(cv::Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false);
    virtual bool mouseMoved(const cv::Point &pos);
    virtual bool mouseReleased(const cv::Point &pos);

    Handle* pt1;
    Handle* pt2;

    virtual void reloadPointers(const std::list<Shape*> &lst, std::list<Shape*>::const_iterator &i);
};

}

/** @example example_shapes.cpp
 * This is an example of how to create shapes by mouse.
 */

#endif // LINE_H
