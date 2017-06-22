#ifndef POLYGON_H
#define POLYGON_H

#include "compoundshape.h"
#include "handle.h"

namespace canvascv
{

class Polygon : public CompoundShape
{
public:
    virtual bool isAtPos(const cv::Point &pos)
    {
        return isPointInPoly(pos);
    }
    virtual std::list<Handle *> getConnectionTargets();
    virtual const char *getType() const;

    virtual bool keyPressed(int &key);

    bool isPointInPoly(const cv::Point &pos) const
    {
        if (isReady())
        {
            return cv::pointPolygonTest(vertices, pos, false) >= 0;

        }
        return false;
    }

    template <typename _TP>
    void getPoints(vector<Point_<_TP>> &out);

    virtual void translate(const cv::Point &offset);

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

#endif // POLYGON_H
