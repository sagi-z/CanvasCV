#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "rectangle.h"

namespace canvasvc
{

class Ellipse : public Rectangle
{
public:
    Ellipse(const cv::Point &pos);

    virtual void draw(Mat &canvas);

    virtual const char *getType() const
    {
        return "Ellipse";
    }
};

}

#endif // ELLIPSE_H
