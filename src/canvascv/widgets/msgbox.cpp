#include "msgbox.h"
#include "text.h"
#include "button.h"
#include "canvascv/canvas.h"

#include <opencv2/highgui.hpp>

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
    frame = VFrame::create(*this, pos).get();
    frame->setFrameRelief(RAISED);
    frame->setPadding(5);
    auto text = Text::create(*frame, "", CENTER_TOP, CENTER);
    text->setAlpha(0); // transpaernt backgound
    buttons = HorizontalLayout::create(*frame).get();
    buttons->setLayoutAnchor(CENTER);
}

void MsgBox::setMsg(const string &msg)
{
    frame->at<Text>(0)->setText(msg);
}

void MsgBox::recalcCompound()
{
    recalcRect();
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
                if (cbUserSelection) cbUserSelection(msgBox.get(), i);
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

int MsgBox::createModal(const string &title, const string &msg, std::vector<string> buttonNames, Widget::CBUserSelection cbUserSelection)
{
    Canvas c(title, Size(1024, 768));
    auto msgBox = create(c, msg, buttonNames, cbUserSelection);
    msgBox->update();
    c.setSize(msgBox->getRect().size());
    msgBox->setLocation({0,0});
    namedWindow(title, WINDOW_AUTOSIZE | WINDOW_GUI_NORMAL);
    c.setMouseCallback();
    int delay = 1000 / 25; // delay because of the polling
    Mat out;
    while(! msgBox->isRemoved())
    {
        c.redrawOn(out);
        c.imshow(out);
        c.waitKeyEx(delay);
    }
    destroyWindow(title);
    return msgBox->getUserSelection();
}

int MsgBox::getUserSelection(bool blocking)
{
    if (blocking)
    {
        Canvas *c = (Canvas*) getLayout();
        int delay = 1000 / 25; // delay because of the polling
        Mat out;
        while(! isRemoved())
        {
            c->redrawOn(out);
            c->imshow(out);
            c->waitKeyEx(delay);
        }
    }
    return userSelection;
}

string MsgBox::getTextAt(int index) const
{
    if (index >= 0 && index < size())
    {
        return buttons->at<Button>(index)->getText();
    }
    return "";
}

size_t MsgBox::size() const
{
    return buttons->size();
}

}
