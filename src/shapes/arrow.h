#ifndef ARROW_H
#define ARROW_H

#include "line.h"

namespace canvascv
{

/**
 * @brief The Arrow class
 * A line in which the head has an arrow shape
 */
class Arrow : public Line
{
public:

    virtual const char *getType() const;

    static const char * type;

protected:
    friend class ShapeFactory;
    template <class T> friend class ShapeFactoryT;

    Arrow(const cv::Point &pos);

    virtual void draw(cv::Mat &canvas);
};

}

/** @example example_shapes.cpp
 * This is an example of how to create shapes by mouse.
 */

#endif // ARROW_H
