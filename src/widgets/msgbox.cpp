#include "msgbox.h"
#include "vframe.h"
#include "horizontallayout.h"
#include "floatingtext.h"
#include "button.h"

namespace canvascv
{

const char *MsgBox::type = "MsgBox";

MsgBox::MsgBox(Layout &layoutVal, const Point &pos)
    :CompoundWidget(layoutVal, pos)
{
    parts = VFrame::create(layoutVal, pos);
    addWidget(parts);
    FloatingText::create(*parts, "");
    buttons = HorizontalLayout::create(*parts);
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
    ((FloatingText*)parts->at(0))->setMsg(msg);
    setDirty();
}

void MsgBox::addButton(const string &buttonText, Widget::CBType cb)
{
    auto button = Button::create(*buttons, buttonText);
    button->notifyOnChange(cb);
    setDirty();
}

}
