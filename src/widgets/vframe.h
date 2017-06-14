#ifndef VFRAME_H
#define VFRAME_H

#include "verticallayout.h"

namespace canvascv
{

class VFrame : public VerticalLayout
{
public:

    VFrame(Layout &layoutVal, const cv::Point &pos);

    static std::shared_ptr<VFrame> create(Layout &layout,
                                          const cv::Point &pos = cv::Point(0,0));

    void setFrameRelief(Widget::Relief value);

    virtual const char *getType() const;

    static const char *type;
};

}
#endif // VFRAME_H
