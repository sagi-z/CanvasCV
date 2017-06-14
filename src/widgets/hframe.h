#ifndef HFRAME_H
#define HFRAME_H

#include "horizontallayout.h"

namespace canvascv
{

class HFrame : public HorizontalLayout
{
public:

    static std::shared_ptr<HFrame> create(Layout &layout,
                                          const cv::Point &pos = cv::Point(0,0));

    void setFrameRelief(Widget::Relief value);

    virtual const char *getType() const;

    static const char *type;

protected:
    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;

    HFrame(Layout &layoutVal, const cv::Point &pos);
};

}
#endif // HFRAME_H
