#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "compoundshape.h"
#include "handle.h"

namespace canvascv
{

/**
 * @brief The Rectangle class
 * Allows you to draw a rotatable rectangle by mouse or from code
 */
class Rectangle : public CompoundShape
{
public:

    /// get the OpenCV representation of this shape
    cv::RotatedRect getRect() const;

    /// set this shape from the OpenCV representation
    void setRect(const cv::RotatedRect &rect);

    virtual bool keyPressed(int &key);

    /// returns true if pos is in the rectangle representated by this shape
    bool isPointInRectangle(cv::Point pos)
    {
        std::vector<Point> pts(4);
        pts[0] = (*pt1)();
        pts[1] = (*pt2)();
        pts[2] = (*pt3)();
        pts[3] = (*pt4)();
        return cv::pointPolygonTest(pts, pos, false) >= 0;
    }

    virtual bool isAtPos(const cv::Point &pos)
    {
        return isPointInRectangle(pos);
    }

    virtual std::list<Handle *> getConnectionTargets();

    virtual void translate(const cv::Point &offset);

    virtual const char *getType() const;

    static const char * type;

protected:
    friend class ShapeFactory;
    template <class T> friend class ShapeFactoryT;

    Rectangle(const cv::Point &pos);

    virtual void draw(cv::Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false);
    virtual bool mouseMoved(const cv::Point &pos);
    virtual bool mouseReleased(const cv::Point &pos);
    virtual const string &getEditStatusMsg() const;

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
