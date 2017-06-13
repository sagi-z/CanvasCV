#ifndef HFRAME_H
#define HFRAME_H

#include "horizontallayout.h"

namespace canvascv
{

class HFrame : public HorizontalLayout
{
public:

    HFrame(const cv::Point &pos);

    static std::shared_ptr<HFrame> create(Layout &layout,
                                          const cv::Point &pos = cv::Point(0,0));

    void setFrameRelief(Widget::Relief value);

    virtual const char *getType() const;

    static const char *type;
};

}
#endif // HFRAME_H
