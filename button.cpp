#include "button.h"
#include "canvas.h"
#include "widgetfactory.h"

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

Button::Button(const string msgVal, Point leftPosVal, int maxWidthVal, Scalar colorVal,
               Scalar bgColorVal, double fontScaleVal, int fontThicknessVal,
               double alphaVal, int fontFaceVal)
    : FloatingText(msgVal, leftPosVal, maxWidthVal, colorVal,
                   bgColorVal, fontScaleVal, fontThicknessVal,
                   alphaVal, fontFaceVal),
      origAlpha(alpha)
{
}

const char *Button::getType() const
{
    return type;
}

shared_ptr<Button> Button::newButton(Canvas &c, Point pos, const string &buttonText, const string &statusMsg, int maxWidthVal)
{
    shared_ptr<Button> widget = c.createWidget<Button>(pos);
    widget->setMsg(buttonText);
    widget->setMaxWidth(maxWidthVal);
    widget->setStatusMsg(statusMsg);
    return widget;
}

std::shared_ptr<Button> Button::newButton(Canvas &c, const string &buttonText, const string &statusMsg, int maxWidthVal)
{
   return newButton(c, Point(0, 0), buttonText, statusMsg, maxWidthVal);
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
