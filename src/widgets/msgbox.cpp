#include "msgbox.h"
#include "floatingtext.h"
#include "button.h"

namespace canvascv
{

const char *MsgBox::type = "MsgBox";

/*
 *  -> VFrame
 *      -> FloatingText (CENTERED)
 *      -> HorizontalLayout (CENTERED)
 *          -> Button (stretchX/Y)
 *          -> Button (stretchX/Y)
 *          ...
 */

MsgBox::MsgBox(Layout &layoutVal, const Point &pos)
    :CompoundWidget(layoutVal, pos)
{
    frame = VFrame::create(layoutVal, pos);
    frame->setFrameRelief(RAISED);
    frame->setPadding(5);
    addWidget(frame);
    auto ft = FloatingText::create(*frame, "", CENTER_TOP, CENTER);
    ft->setFillBG(false); // no seperate background to the text
    buttons = HorizontalLayout::create(*frame);
    buttons->setLayoutAnchor(CENTER);
}

const char *MsgBox::getType() const
{
    return type;
}

std::shared_ptr<MsgBox> MsgBox::create(Layout &layout, const string &msg, const Point &pos)
{
    shared_ptr<MsgBox> widget(WidgetFactoryT<MsgBox>::newWidget(layout, pos));
    widget->setMsg(msg);
    return widget;
}

void MsgBox::setMsg(const string &msg)
{
    frame->at<FloatingText>(0)->setMsg(msg);
    setDirty();
}

void MsgBox::addButton(const string &buttonText, Widget::CBType cb)
{
    auto button = Button::create(*buttons, buttonText);
    button->setStretchX(true);
    button->setStretchY(true);
    button->notifyOnChange(cb);
    setDirty();
}

}
