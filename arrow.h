#ifndef ARROW_H
#define ARROW_H

#include "line.h"

namespace canvascv
{

class Arrow : public Line
{
public:
    Arrow(const cv::Point &pos)
        : Line(pos) {}

    virtual void draw(cv::Mat &canvas);

    virtual const char *getType() const
    {
        return type;
    }

    static const char * type;
};

}

#endif // ARROW_H
