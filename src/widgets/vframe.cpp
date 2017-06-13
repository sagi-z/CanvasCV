#include "vframe.h"

namespace canvascv
{

const char * VFrame::type = "VFrame";

VFrame::VFrame(const Point &pos)
    : VerticalLayout(pos)
{
    setDrawFrame(true);
}

std::shared_ptr<VFrame> VFrame::create(Layout &layout, const Point &pos)
{
    shared_ptr<VFrame> widget(WidgetFactoryT<VFrame>::newWidget(pos));
    layout.addWidget(widget);
    return widget;
}

void VFrame::setFrameRelief(Widget::Relief value)
{
    setRelief(value);
}

const char *VFrame::getType() const
{
    return type;
}

}
