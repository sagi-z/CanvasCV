#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "compoundshape.h"
#include "handle.h"

namespace canvascv
{

class Rectangle : public CompoundShape
{
public:
    Rectangle(const cv::Point &pos);

    virtual bool isAtPos(const cv::Point &pos)
    {
        return isPointInRectangle(pos);
    }

    virtual std::list<Handle *> getConnectionTargets();
    virtual const char *getType() const;

    cv::RotatedRect getRect() const;

    void setRect(const cv::RotatedRect &rect);

    bool isPointInRectangle(cv::Point pos)
    {
        std::vector<Point> pts(4);
        pts[0] = (*pt1)();
        pts[1] = (*pt2)();
        pts[2] = (*pt3)();
        pts[3] = (*pt4)();
        return cv::pointPolygonTest(pts, pos, false) >= 0;
    }

    static const char * type;

    virtual void translate(const cv::Point &offset);

protected:
    virtual void draw(cv::Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false);
    virtual bool mouseMoved(const cv::Point &pos);
    virtual bool mouseReleased(const cv::Point &pos);

private:
    void recalcRect(const cv::Point &pos, bool rotated = false, float offset = 0);

    void updatePoints();

    void registerCBs();

    Handle* pt1;
    Handle* pt2;
    Handle* pt3;
    Handle* pt4;
    Handle* rotate1;
    Handle* rotate2;
    Handle* rotate3;
    Handle* rotate4;
    Handle* center;
    float width;
    float height;
    float angle; // in radians
    bool selfUpdate;

    virtual void writeInternals(cv::FileStorage &fs) const;
    virtual void readInternals(const cv::FileNode &node);

    virtual void reloadPointers(const std::list<Shape*> &lst, std::list<Shape*>::const_iterator &i);
};

}
#endif // RECTANGLE_H
