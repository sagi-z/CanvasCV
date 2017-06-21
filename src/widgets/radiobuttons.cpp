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

RadioButtons::RadioButtons(Layout &layoutVal, const Point &pos)
    :CompoundWidget(layoutVal, pos),
      selection(-1)
{
    frame = VFrame::create(*this, pos);
    frame->setFrameRelief(RAISED);
    frame->setPadding(2);
    frame->setSpacing(1);
}

void RadioButtons::addRadioButton(const string &txt, Widget::CBUserSelection cbUserSelection)
{
    int i = frame->size();
    auto row = HorizontalLayout::create(*frame);
    auto button = MatWidget::create(*row, getRadioNotSelected());
    auto text = Text::create(*row, txt);
    button->setLayoutAnchor(CENTER);
    text->setFlowAnchor(LEFT);
    text->setLayoutAnchor(CENTER);
    text->setAlpha(0);
    Widget::CBWidgetState pressCB = [button, this, i, cbUserSelection](Widget *, Widget::State state) {
        if (state == Widget::PRESS) {
            this->setSelection(i);
            if (cbUserSelection) cbUserSelection(this, i);
        }
    };
    button->notifyOnChange(pressCB);
    text->notifyOnChange(pressCB);
    drawings.push_back(button.get());
    setDirty();
}

void RadioButtons::recalc()
{
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
    }
    CompoundWidget::recalc();
}

cv::Mat RadioButtons::getRadioSelected()
{
    Mat drawing(12,12,CV_8UC4);
    drawing = Scalar::all(0); // transparent BG
    circle(drawing, Point(5,5), 5, getOutlineColor(), -1, lineType);
    return drawing;
}

cv::Mat RadioButtons::getRadioNotSelected()
{
    Mat drawing(12,12,CV_8UC4);
    drawing = Scalar::all(0); // transparent BG
    // FIXME: value of 2 here is strange
    circle(drawing, Point(5,5), 5, getOutlineColor(), 2, lineType);
    return drawing;
}

int RadioButtons::getSelection() const
{
    return selection;
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

    for (int i = 0; i < buttonNames.size(); ++i)
    {
        radioButtons->addRadioButton(buttonNames[i], cbUserSelection);
    }

    radioButtons->setSelection(defaultSelection);

    return radioButtons;
}

void RadioButtons::setSelection(int value)
{
   if (value != selection && value >= 0 && value < frame->size())
   {
       selection = value;
       setDirty();
   }
}

}
