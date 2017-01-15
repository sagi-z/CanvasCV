#ifndef ARROW_H
#define ARROW_H

#include "line.h"

namespace canvasvc
{

class Arrow : public Line
{
public:
    Arrow(const cv::Point &pos)
        : Line(pos) {}

    virtual void draw(cv::Mat &canvas);

    virtual const char *getType() const
    {
        return "Arrow";
    }
};

}

#endif // ARROW_H
