#ifndef POLYGON_H
#define POLYGON_H

#include "compoundshape.h"
#include "handle.h"

namespace canvascv
{

class Polygon : public CompoundShape
{
public:
    Polygon(const cv::Point &pos);

    virtual bool isAtPos(const cv::Point &pos)
    {
        return isPointInPoly(pos);
    }
    virtual std::list<Handle *> getConnectionTargets();
    virtual const char *getType() const;

    virtual bool keyPressed(int &key);

    virtual const std::string &getStatusMsg() const;

    bool isPointInPoly(const cv::Point &pos) const
    {
        if (ready)
        {
            return cv::pointPolygonTest(vertices, pos, false) >= 0;

        }
        return false;
    }

    template <typename _TP>
    void getPoints(vector<Point_<_TP>> &out);

    static const char * type;

protected:

    virtual void draw(cv::Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false);
    virtual bool mouseMoved(const cv::Point &pos);
    virtual bool mouseReleased(const cv::Point &pos);
    virtual void lostFocus();

private:

    virtual void reloadPointers(const std::list<Shape*> &lst, std::list<Shape*>::const_iterator &i);
    bool ready;
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
