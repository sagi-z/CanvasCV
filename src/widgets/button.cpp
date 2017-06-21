#include "button.h"
#include "widgetfactory.h"
#include "layout.h"

using namespace cv;
using namespace std;

namespace canvascv
{

const char *Button::type = "Button";

Button::Button(Layout &layoutVal, const Point &pos)
    : Text(layoutVal, pos)
{
    setRelief(RAISED);
    setFlowAnchor(CENTER_TOP);
}

const char *Button::getType() const
{
    return type;
}

shared_ptr<Button> Button::create(Layout &layout,
                                  const Point &pos,
                                  const string &buttonText,
                                  const string &statusMsg,
                                  int maxWidthVal)
{
    shared_ptr<Button> widget(WidgetFactoryT<Button>::newWidget(layout, pos));
    widget->setMsg(buttonText);
    widget->setMaxWidth(maxWidthVal);
    widget->setStatusMsg(statusMsg);
    return widget;
}

std::shared_ptr<Button> Button::create(Layout &layout, const string &buttonText, const string &statusMsg, int maxWidthVal)
{
    return create(layout, Point(0, 0), buttonText, statusMsg, maxWidthVal);
}

void Button::setFlatButton()
{
   setRelief(FLAT);
}

void Button::mousePressed()
{
    if (relief == FLAT)
    {
        flatWidget();
    }
    else
    {
        sunkenWidget();
    }
}

void Button::mouseReleased()
{
    if (relief == FLAT)
    {
        flatWidget();
    }
    else
    {
        raisedWidget();
    }
}

void Button::mouseEnter()
{
    selectedWidget();
}

void Button::mouseLeave()
{
    if (relief == FLAT)
    {
        flatWidget();
    }
    else
    {
        raisedWidget();
    }
}

}
