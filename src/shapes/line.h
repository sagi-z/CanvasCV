#ifndef LINE_H
#define LINE_H

#include "compoundshape.h"
#include "shapefactory.h"
#include "handle.h"

#include <opencv2/highgui.hpp>

namespace canvascv
{

class Line : public CompoundShape
{
public:
    Line(const cv::Point &pos);

    virtual std::list<Handle *> getConnectionTargets();

    virtual const char *getType() const {
        return type;
    }

    void lockTail(bool isLocked);

    void lockHead(bool isLocked);

    void showTail(bool isVisible);

    void showHead(bool isVisible);

    void setTailPos(const cv::Point& pos);

    void setHeadPos(const cv::Point& pos);

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

    Handle &getPT1();

    Handle &getPT2();

    const cv::Point &getTail() const;

    const cv::Point &getHead() const;

    virtual bool isAtPos(const cv::Point &pos)
    {
        return isPointOnLine(pos);
    }

    virtual bool keyPressed(int &key);

    static const char * type;

protected:
    virtual void draw(cv::Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false);
    virtual bool mouseMoved(const cv::Point &pos);
    virtual bool mouseReleased(const cv::Point &pos);

    Handle* pt1;
    Handle* pt2;

    virtual void reloadPointers(const std::list<Shape*> &lst, std::list<Shape*>::const_iterator &i);
};

}

#endif // LINE_H
