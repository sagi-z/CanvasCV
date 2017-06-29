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

/** @example example_shapes_widgets.cpp
 * This is an example of how to create shapes by code and mouse.
 */

#endif // ELLIPSE_H
