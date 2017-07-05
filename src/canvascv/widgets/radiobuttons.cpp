#include "widgetfactory.h"
#include "radiobuttons.h"
#include "horizontallayout.h"
#include "text.h"
#include "matwidget.h"

using namespace std;
using namespace cv;

namespace canvascv
{

const char *RadioButtons::type = "RadioButtons";

/*
 *  -> VFrame
 *      -> HorizontalLayout
 *          |MatWidget|text|
 *      ...
 */

RadioButtons::RadioButtons(const Point &pos)
    :CompoundWidget(pos),
      selection(-1)
{
    frame = VFrame::create(*this, pos);
    frame->setFrameRelief(RAISED);
    frame->setPadding(2);
    frame->setSpacing(1);
}

void RadioButtons::addRadioButton(const string &txt)
{
    int i = frame->size();
    auto row = HorizontalLayout::create(*frame);
    auto button = MatWidget::create(*row, getRadioNotSelected());
    auto text = Text::create(*row, txt);
    button->setLayoutAnchor(CENTER);
    text->setAlpha(0);
    text->setFlowAnchor(LEFT);
    text->setLayoutAnchor(CENTER);
    text->setAlpha(0);
    Widget::CBWidgetState pressCB = [this, i](Widget *, Widget::State state) {
        if (state == Widget::PRESS) {
            this->setSelection(i);
        }
    };
    button->notifyOnChange(pressCB);
    text->notifyOnChange(pressCB);
    drawings.push_back(button.get());
    setDirty();
}

void RadioButtons::recalcCompound()
{
    radioSelected.release();
    radioNotSelected.release();
    for (int i = 0; i < drawings.size(); ++i)
    {
        if (i == selection)
        {
            drawings[i]->setMat(getRadioSelected());
        }
        else
        {
            drawings[i]->setMat(getRadioNotSelected());
        }
        drawings[i]->update();
    }
    recalcRect();
}

cv::Mat RadioButtons::getRadioSelected()
{
    if (radioSelected.empty())
    {
        radioSelected = Mat::zeros(14,14,CV_8UC4);
        circle(radioSelected, Point(6,6), 4, getOutlineColor(), -1, lineType);
    }
    return radioSelected;
}

cv::Mat RadioButtons::getRadioNotSelected()
{
    if (radioNotSelected.empty())
    {
        radioNotSelected = Mat::zeros(14,14,CV_8UC4);
        circle(radioNotSelected, Point(6,6), 4, getOutlineColor(), thickness, lineType);
    }
    return radioNotSelected;
}

int RadioButtons::getSelection() const
{
    return selection;
}

string RadioButtons::getTextAt(int index) const
{
    if (index >= 0 && index < size())
    {
        return frame->at<HorizontalLayout>(index)->at<Text>(1)->getText();
    }
    return "";
}

size_t RadioButtons::size() const
{
    return frame->size();
}

void RadioButtons::setUserCB(Widget::CBUserSelection cbUserSelection)
{
    userCB  = cbUserSelection;
}

const char *RadioButtons::getType() const
{
    return type;
}

shared_ptr<RadioButtons> RadioButtons::create(Layout &layoutVal, vector<string> buttonNames, int defaultSelection,
                                              CBUserSelection cbUserSelection, const Point &pos)
{
    assert(buttonNames.size());
    shared_ptr<RadioButtons> radioButtons(WidgetFactoryT<RadioButtons>::newWidget(layoutVal, pos));
    radioButtons->setUserCB(cbUserSelection);

    for (int i = 0; i < buttonNames.size(); ++i)
    {
        radioButtons->addRadioButton(buttonNames[i]);
    }

    radioButtons->setSelection(defaultSelection);

    return radioButtons;
}

void RadioButtons::setSelection(int value)
{
   if (value != selection && value >= 0 && value < frame->size())
   {
       selection = value;
       if (userCB) userCB(this, value);
       setDirty();
   }
}

}
