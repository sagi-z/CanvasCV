#ifndef ARROW_H
#define ARROW_H

#include "line.h"

namespace canvascv
{

class Arrow : public Line
{
public:
    Arrow(const cv::Point &pos);

    virtual const char *getType() const;

    static const char * type;

protected:

    virtual void draw(cv::Mat &canvas);
};

}

#endif // ARROW_H
