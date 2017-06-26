#include "msgbox.h"
#include "text.h"
#include "button.h"
#include "canvas.h"
using namespace std;

using namespace cv;

namespace canvascv
{

const char *MsgBox::type = "MsgBox";

/*
 *  -> VFrame
 *      -> Text (CENTERED)
 *      -> HorizontalLayout (CENTERED)
 *          -> Button (stretchX/Y)
 *          -> Button (stretchX/Y)
 *          ...
 */

MsgBox::MsgBox(const Point &pos)
    :CompoundWidget(pos),
      userSelection(-1)
{
    frame = VFrame::create(*this, pos);
    frame->setFrameRelief(RAISED);
    frame->setPadding(5);
    auto text = Text::create(*frame, "", CENTER_TOP, CENTER);
    text->setAlpha(0); // transpaernt backgound
    buttons = HorizontalLayout::create(*frame);
    buttons->setLayoutAnchor(CENTER);
}

void MsgBox::setMsg(const string &msg)
{
    frame->at<Text>(0)->setMsg(msg);
}

void MsgBox::recalcCompound()
{
}

const char *MsgBox::getType() const
{
    return type;
}

shared_ptr<MsgBox> MsgBox::create(Canvas &canvas, const string &msg, vector<string> buttonNames,
                                       CBUserSelection cbUserSelection, const Point &pos)
{
    shared_ptr<MsgBox> msgBox(WidgetFactoryT<MsgBox>::newWidget(canvas, pos));
    msgBox->setMsg(msg);

    for (int i = 0; i < buttonNames.size(); ++i)
    {
        auto button = Button::create(*msgBox->buttons, buttonNames[i]);
        button->setStretchX(true);
        button->setStretchY(true);
        button->notifyOnChange([msgBox, i, cbUserSelection](Widget *w, Widget::State state) {
            if (state == Widget::PRESS) {
                msgBox->userSelection = i;
                if (cbUserSelection) cbUserSelection(w, i);
                msgBox->rmvFromLayout();
            }
        });
    }

    // this part could have been done by implementing 'recalcCompound' - START
    msgBox->update();
    if (pos.x < 0 || pos.y < 0)
    {
        Size screenSize = canvas.getSize();
        msgBox->setLocation(Point(screenSize.width / 2. - msgBox->getRect().width / 2.,
                                  screenSize.height / 2. - msgBox->getRect().height / 2.));
    }
    // this part could have been done by implementing 'recalcCompound' - END


    return msgBox;
}

int MsgBox::getUserSelection()
{
    return userSelection;
}

}
