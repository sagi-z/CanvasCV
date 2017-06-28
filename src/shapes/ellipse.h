#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "rectangle.h"

namespace canvascv
{

/**
 * @brief The Ellipse class
 * Allows you to draw a rotatable ellipse by mouse or from code
 */
class Ellipse : public Rectangle
{
public:

    virtual const char *getType() const;

    static const char * type;

protected:
    friend class ShapeFactory;
    template <class T> friend class ShapeFactoryT;

    Ellipse(const cv::Point &pos);
    virtual void draw(Mat &canvas);
};

}

#endif // ELLIPSE_H
