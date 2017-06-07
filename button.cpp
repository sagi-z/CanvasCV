#include "button.h"
#include "widgetfactory.h"
#include "layout.h"

using namespace cv;
using namespace std;

namespace canvascv
{

const char *Button::type = "Button";

Button::Button(const Point &pos)
    : FloatingText(pos),
      origAlpha(alpha)
{
}

Button::Button(const string msgVal, Point locationVal, int maxWidthVal, Scalar colorVal,
               Scalar bgColorVal, double fontScaleVal, int fontThicknessVal, int fontFaceVal)
    : FloatingText(msgVal, locationVal, maxWidthVal, colorVal,
                   bgColorVal, fontScaleVal, fontThicknessVal,
                   fontFaceVal),
      origAlpha(alpha)
{
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
    shared_ptr<Button> widget(WidgetFactoryT<Button>::newWidget(pos));
    widget->setMsg(buttonText);
    widget->setMaxWidth(maxWidthVal);
    widget->setStatusMsg(statusMsg);
    layout.addWidget(widget);
    return widget;
}

std::shared_ptr<Button> Button::create(Layout &layout, const string &buttonText, const string &statusMsg, int maxWidthVal)
{
    return create(layout, Point(0, 0), buttonText, statusMsg, maxWidthVal);
}

void Button::setAlpha(double val)
{
    FloatingText::setAlpha(val);
    origAlpha = alpha;
}

void Button::mousePressed()
{
   alpha = 0.2;
}

void Button::mouseReleased()
{
    mouseEnter();
}

void Button::mouseEnter()
{
    alpha = 0.8;
}

void Button::mouseLeave()
{
    alpha = origAlpha;
}

}
