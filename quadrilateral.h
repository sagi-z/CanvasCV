#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H

#include "compoundshape.h"
#include "handle.h"

namespace canvascv
{

class Quadrilateral : public CompoundShape
{
public:
    Quadrilateral(const cv::Point &pos)
        : selectedPoint1(0),
        selectedPoint2(0)
    {
        for (int i = 0; i<4; ++i)
        {
            pts[i] = 0;
        }
        pts[0] = addShape<Handle>(pos);
        setOutlineColor(Colors::P1_LBLUE);
        setThickness(2);
        pts[0]->setThickness(3);
    }

    virtual void draw(cv::Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false);
    virtual bool mouseMoved(const cv::Point &pos);
    virtual bool mouseReleased(const cv::Point &pos);
    virtual bool isAtPos(const cv::Point &pos)
    {
        std::vector<Point> vertices(4);
        vertices[0] = (*pts[0])();
        vertices[1] = (*pts[1])();
        vertices[2] = (*pts[2])();
        vertices[3] = (*pts[3])();
        return cv::pointPolygonTest(vertices, pos, false) >= 0;
    }
    virtual std::list<Handle *> getConnectionTargets();
    virtual const char *getType() const
    {
        return type;
    }

    bool isPointOnLine(const cv::Point &p3, Handle *&pt1, Handle *&pt2, int threshold=3) const {
        if (! pts[3])
        {
            return false;
        }
        for (int i = 0; i<4; ++i)
        {
            threshold += thickness/2;
            int first = i;
            int second = i+1;
            if (second == 4)
            {
                second = 0;
            }
            const cv::Point &p1 = (*pts[first])();
            const cv::Point &p2 = (*pts[second])();
            double len =  sqrt(pow(p2.y-p1.y,2)+pow(p2.x-p1.x,2));

            // Make sure it is roughly on the line
            if (threshold <= abs((p2.y-p1.y)*p3.x-(p2.x-p1.x)*p3.y+p2.x*p1.y-p2.y*p1.x) / len)
            {
                continue;
            }

            // For p3 to be in p1<->p2 range, distance from p3 to either p1 or p2
            //  cannot be more than distance from p1 to p2 (len).

            if(sqrt(pow(p3.y-p1.y,2)+pow(p3.x-p1.x,2)) > len)
            {
                continue;
            }

            if(sqrt(pow(p3.y-p2.y,2)+pow(p3.x-p2.x,2)) > len)
            {
                continue;
            }

            pt1 = pts[first];
            pt2 = pts[second];
            return true;
        }

        return false;
    }

    template <typename _TP>
    void getPoints(vector<Point_<_TP>> &out);

    static const char * type;

private:
    virtual void reloadPointers(std::list<Shape*>::const_iterator &i)
    {
        CompoundShape::reloadPointers(i);
        for (int j = 0; j<4; ++j)
        {
            pts[j] = dynamic_cast<Handle*>(*i++);
        }
    }
    Handle* pts[4];
    Handle *selectedPoint1;
    Handle *selectedPoint2;
};

template <typename _TP>
void Quadrilateral::getPoints(vector<Point_<_TP> > &out)
{
    out.resize(4);
    for (int i = 0; i<4; ++i)
    {
        assert(pts[i]);
        out[i] = (*pts[i])();
    }
}

}

#endif // QUADRILATERAL_H
