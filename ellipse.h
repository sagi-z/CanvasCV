#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "rectangle.h"

namespace canvascv
{

class Ellipse : public Rectangle
{
public:
    Ellipse(const cv::Point &pos);

    virtual const char *getType() const;

    static const char * type;

protected:
    virtual void draw(Mat &canvas);
};

}

#endif // ELLIPSE_H
