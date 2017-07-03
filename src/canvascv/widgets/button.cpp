#include "button.h"
#include "widgetfactory.h"
#include "layout.h"

using namespace cv;
using namespace std;

namespace canvascv
{

const char *Button::type = "Button";

Button::Button(const Point &pos)
    : Text(pos)
{
    setRelief(RAISED);
    setFlowAnchor(CENTER_TOP);
    notifyOnChange([this](Widget*, State state)
    {
       if (state == PRESS && cb) cb(this);
    });
}

const char *Button::getType() const
{
    return type;
}

shared_ptr<Button> Button::create(Layout &layout,
                                  const Point &pos,
                                  const string &buttonText,
                                  const string &statusMsg,
                                  CBWidget cbVal)
{
    shared_ptr<Button> widget(WidgetFactoryT<Button>::newWidget(layout, pos));
    widget->setText(buttonText);
    widget->setStatusMsg(statusMsg);
    widget->onPress(cbVal);
    return widget;
}

std::shared_ptr<Button> Button::create(Layout &layout, const string &buttonText, const string &statusMsg, CBWidget cbVal)
{
    return create(layout, Point(0, 0), buttonText, statusMsg, cbVal);
}

void Button::setFlatButton()
{
    setRelief(FLAT);
}

void Button::onPress(Button::CBWidget value)
{
    cb = value;
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
