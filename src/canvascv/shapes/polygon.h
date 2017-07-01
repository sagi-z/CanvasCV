#ifndef POLYGON_H
#define POLYGON_H

#include "compoundshape.h"
#include "handle.h"

namespace canvascv
{

/**
 * @brief The Polygon class
 * 
 * Allows you to draw a polygon by mouse or from code
 * @sa
 */
class Polygon : public CompoundShape
{
public:

    /// returns true if pos is in the polygon
    bool isPointInPoly(const cv::Point &pos) const
    {
        if (isReady())
        {
            return cv::pointPolygonTest(vertices, pos, false) >= 0;

        }
        return false;
    }

    /**
     * @brief getPoints
     * 
     * return the vertices of the polygon
     * @param out will contain the vertices on return
     */
    template <typename _TP>
    void getPoints(vector<Point_<_TP>> &out);

    virtual bool isAtPos(const cv::Point &pos)
    {
        return isPointInPoly(pos);
    }
    virtual std::list<Handle *> getConnectionTargets();

    virtual bool keyPressed(int &key);
    virtual void translate(const cv::Point &offset);

    virtual const char *getType() const;

    static const char * type;

protected:

    friend class ShapeFactory;
    template <class T> friend class ShapeFactoryT;

    Polygon(const cv::Point &pos);

    virtual void draw(cv::Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false);
    virtual bool mouseMoved(const cv::Point &pos);
    virtual bool mouseReleased(const cv::Point &pos);
    virtual void lostFocus();
    virtual const string &getCreateStatusMsg() const;
    virtual const string &getEditStatusMsg() const;
private:

    virtual void reloadPointers(const std::list<Shape*> &lst, std::list<Shape*>::const_iterator &i);
    std::list<Handle*> handles;
    std::vector<cv::Point> vertices;
};

template <typename _TP>
void Polygon::getPoints(vector<Point_<_TP> > &out)
{
    out.clear();
    for (Handle *handle : handles)
    {
        out.push_back((*handle)());
    }
}

}

/** @example example_shapes.cpp
 * This is an example of how to create shapes by mouse.
 */

#endif // POLYGON_H
