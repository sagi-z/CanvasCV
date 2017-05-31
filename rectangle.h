#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "compoundshape.h"
#include "handle.h"

namespace canvascv
{

class Rectangle : public CompoundShape
{
public:
    Rectangle(const cv::Point &pos)
    {
        pt1 = addShape<Handle>(cv::Point(pos.x-1, pos.y-1));
        pt2 = addShape<Handle>(cv::Point(pos.x+1, pos.y-1));
        pt3 = addShape<Handle>(cv::Point(pos.x+1, pos.y+1));
        pt4 = addShape<Handle>(cv::Point(pos.x-1, pos.y+1));
        rotate1 = addShape<Handle>(cv::Point(pos.x+1, pos.y));
        rotate2 = addShape<Handle>(cv::Point(pos.x, pos.y+1));
        rotate3 = addShape<Handle>(cv::Point(pos.x-1, pos.y));
        rotate4 = addShape<Handle>(cv::Point(pos.x, pos.y-1));
        center = addShape<Handle>(pos);
        width = 3;
        height = 3;
        angle = 0;
        selfUpdate = false;
        updatePoints();
        registerCBs();
    }

    virtual void draw(cv::Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false);
    virtual bool mouseMoved(const cv::Point &pos);
    virtual bool mouseReleased(const cv::Point &pos);
    virtual bool isAtPos(const cv::Point &pos)
    {
        return isPointInRectangle(pos);
    }

    virtual std::list<Handle *> getConnectionTargets();
    virtual const char *getType() const {
        return type;
    }

    cv::RotatedRect getRect() const
    {
        return cv::RotatedRect((*center)(), cv::Size(width, height), angle * 180 / CV_PI);
    }

    void setRect(const cv::RotatedRect &rect)
    {
        center->setPos(rect.center);
        width = rect.size.width;
        height = rect.size.height;
        angle = rect.angle * CV_PI / 180;
        updatePoints();
    }

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

private:
    void recalcRect(const cv::Point &pos, bool rotated = false, float offset = 0)
    {
        if (rotated)
        {
            const cv::Point &centerPoint = (*center)();
            angle = atan2(pos.y - centerPoint.y, pos.x - centerPoint.x);
            angle -= offset;
        }
        else
        {
            const cv::Point &centerPoint = (*center)();
            float distance = sqrt(pow(pos.x-centerPoint.x,2)+pow(pos.y-centerPoint.y,2));
            float angleToPoint = atan2(pos.y - centerPoint.y, pos.x - centerPoint.x);
            angleToPoint -= angle;
            width = abs(cos(angleToPoint)*distance*2);
            height = abs(sin(angleToPoint)*distance*2);
        }

        updatePoints();
    }

    void updatePoints()
    {
        float fSin = sin(angle)*0.5f;
        float fCos = cos(angle)*0.5f;
        const cv::Point &centerPoint = (*center)();

        cv::Point pt;

        pt.x = centerPoint.x + cos(angle) * width * 0.5f;
        pt.y = centerPoint.y + sin(angle) * width * 0.5f;
        rotate1->setPos(pt);

        pt.x = centerPoint.x + cos(angle+CV_PI/2) * height * 0.5f;
        pt.y = centerPoint.y + sin(angle+CV_PI/2) * height * 0.5f;
        rotate2->setPos(pt);

        pt.x = centerPoint.x + cos(angle+CV_PI) * width * 0.5f;
        pt.y = centerPoint.y + sin(angle+CV_PI) * width * 0.5f;
        rotate3->setPos(pt);

        pt.x = centerPoint.x + cos(angle+CV_PI*1.5) * height * 0.5f;
        pt.y = centerPoint.y + sin(angle+CV_PI*1.5) * height * 0.5f;
        rotate4->setPos(pt);

        pt.x = centerPoint.x - fSin*height - fCos*width;
        pt.y = centerPoint.y + fCos*height - fSin*width;
        pt1->setPos(pt);

        pt.x = centerPoint.x + fSin*height - fCos*width;
        pt.y = centerPoint.y - fCos*height - fSin*width;
        pt2->setPos(pt);

        pt.x = 2*centerPoint.x - (*pt1)().x;
        pt.y = 2*centerPoint.y - (*pt1)().y;
        pt3->setPos(pt);

        pt.x = 2*centerPoint.x - (*pt2)().x;
        pt.y = 2*centerPoint.y - (*pt2)().y;
        pt4->setPos(pt);
    }

    void registerCBs()
    {
        for (auto &pt : {pt1, pt2, pt3, pt4})
        {
            pt->addPosChangedCB([this](const cv::Point &pos)
            {
                if (!selfUpdate)
                {
                    selfUpdate = true;
                    recalcRect(pos);
                    selfUpdate = false;
                }
            });
        }
        rotate1->addPosChangedCB([this](const cv::Point &pos)
        {
                if (!selfUpdate)
                {
                    selfUpdate = true;
                    recalcRect(pos, true, 0);
                    selfUpdate = false;
                }
        });
        rotate2->addPosChangedCB([this](const cv::Point &pos)
        {
                if (!selfUpdate)
                {
                    selfUpdate = true;
                    recalcRect(pos, true, CV_PI/2);
                    selfUpdate = false;
                }
        });
        rotate3->addPosChangedCB([this](const cv::Point &pos)
        {
                if (!selfUpdate)
                {
                    selfUpdate = true;
                    recalcRect(pos, true, CV_PI);
                    selfUpdate = false;
                }
        });
        rotate4->addPosChangedCB([this](const cv::Point &pos)
        {
                if (!selfUpdate)
                {
                    selfUpdate = true;
                    recalcRect(pos, true, CV_PI*1.5);
                    selfUpdate = false;
                }
        });
        center->addPosChangedCB([this](const cv::Point)
        {
                if (!selfUpdate)
                {
                    selfUpdate = true;
                    updatePoints();
                    selfUpdate = false;
                }
        });
    }

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

    virtual void reloadPointers(std::list<Shape*>::const_iterator &i)
    {
        CompoundShape::reloadPointers(i);
        pt1 = dynamic_cast<Handle*>(*i++);
        pt2 = dynamic_cast<Handle*>(*i++);
        pt3 = dynamic_cast<Handle*>(*i++);
        pt4 = dynamic_cast<Handle*>(*i++);
        rotate1 = dynamic_cast<Handle*>(*i++);
        rotate2 = dynamic_cast<Handle*>(*i++);
        rotate3 = dynamic_cast<Handle*>(*i++);
        rotate4 = dynamic_cast<Handle*>(*i++);
        center = dynamic_cast<Handle*>(*i++);
        registerCBs();
    }
};

}
#endif // RECTANGLE_H
