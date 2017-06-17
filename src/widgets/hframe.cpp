#include "hframe.h"
#include "widgetfactory.h"

using namespace cv;
using namespace std;

namespace canvascv
{

const char * HFrame::type = "HFrame";

HFrame::HFrame(Layout &layoutVal, const Point &pos)
    : HorizontalLayout(layoutVal, pos)
{
    setDrawFrame(true);
}

shared_ptr<HFrame> HFrame::create(Layout &layout, const Point &pos)
{
    shared_ptr<HFrame> widget(WidgetFactoryT<HFrame>::newWidget(layout, pos));
    return widget;
}

void HFrame::setFrameRelief(Relief value)
{
    setRelief(value);
}

const char *HFrame::getType() const
{
    return type;
}

}
