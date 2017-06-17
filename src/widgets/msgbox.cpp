#include "msgbox.h"
#include "floatingtext.h"
#include "button.h"
#include "canvas.h"
using namespace std;

using namespace cv;

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
    :CompoundWidget(layoutVal, pos),
      userSelection(-1)
{
    frame = VFrame::create(*this, pos);
    frame->setFrameRelief(RAISED);
    frame->setPadding(5);
    auto text = FloatingText::create(*frame, "", CENTER_TOP, CENTER);
    text->setFillBG(false); // no seperate background to the text
    buttons = HorizontalLayout::create(*frame);
    buttons->setLayoutAnchor(CENTER);
}

void MsgBox::setMsg(const string &msg)
{
   frame->at<FloatingText>(0)->setMsg(msg);
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

    // this part could have been done by implementing 'recalc' - START
    msgBox->update();
    if (pos.x < 0 || pos.y < 0)
    {
        Size screenSize = canvas.getSize();
        msgBox->setLocation(Point(screenSize.width / 2. - msgBox->getRect().width / 2.,
                                  screenSize.height / 2. - msgBox->getRect().height / 2.));
    }
    // this part could have been done by implementing 'recalc' - END


    return msgBox;
}

int MsgBox::getUserSelection()
{
    return userSelection;
}

}
