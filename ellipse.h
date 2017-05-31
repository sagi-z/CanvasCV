#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "rectangle.h"

namespace canvascv
{

class Ellipse : public Rectangle
{
public:
    Ellipse(const cv::Point &pos);

    virtual void draw(Mat &canvas);

    virtual const char *getType() const
    {
        return type;
    }

    static const char * type;
};

}

#endif // ELLIPSE_H
