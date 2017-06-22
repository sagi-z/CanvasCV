#ifndef ARROW_H
#define ARROW_H

#include "line.h"

namespace canvascv
{

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

#endif // ARROW_H
