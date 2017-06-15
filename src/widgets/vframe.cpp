#include "vframe.h"

namespace canvascv
{

const char * VFrame::type = "VFrame";

VFrame::VFrame(Layout &layoutVal, const Point &pos)
    : VerticalLayout(layoutVal, pos)
{
    setDrawFrame(true);
}

std::shared_ptr<VFrame> VFrame::create(Layout &layout, const Point &pos)
{
    shared_ptr<VFrame> widget(WidgetFactoryT<VFrame>::newWidget(layout, pos));
    return widget;
}

void VFrame::setFrameRelief(Relief value)
{
    setRelief(value);
}

const char *VFrame::getType() const
{
    return type;
}

}
