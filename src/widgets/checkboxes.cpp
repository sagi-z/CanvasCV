#include "widgetfactory.h"
#include "checkboxes.h"
#include "horizontallayout.h"
#include "text.h"
#include "matwidget.h"

using namespace std;
using namespace cv;

namespace canvascv
{

const char *CheckBoxes::type = "CheckBoxes";

/*
 *  -> VFrame
 *      -> HorizontalLayout
 *          |MatWidget|text|
 *      ...
 */

CheckBoxes::CheckBoxes(Layout &layoutVal, const Point &pos)
    :CompoundWidget(layoutVal, pos)
{
    frame = VFrame::create(*this, pos);
    frame->setFrameRelief(RAISED);
    frame->setPadding(2);
    frame->setSpacing(1);
}

void CheckBoxes::addCheckBox(const string &txt, Widget::CBUserSelection cbUserSelection)
{
    int i = frame->size();
    auto row = HorizontalLayout::create(*frame);
    auto checkBox = MatWidget::create(*row, getCheckBoxNotSelected());
    auto text = Text::create(*row, txt);
    checkBox->setLayoutAnchor(CENTER);
    text->setAlpha(0);
    text->setFlowAnchor(LEFT);
    text->setLayoutAnchor(CENTER);
    text->setAlpha(0);
    Widget::CBWidgetState pressCB = [checkBox, this, i, cbUserSelection](Widget *, Widget::State state) {
        if (state == Widget::PRESS) {
            bool isChecked = ! this->isChecked(i);
            this->setChecked(i, isChecked);
            if (cbUserSelection) cbUserSelection(this, i);
        }
    };
    checkBox->notifyOnChange(pressCB);
    text->notifyOnChange(pressCB);
    drawings.push_back(checkBox.get());
    selections.push_back(false);
    setDirty();
}

void CheckBoxes::recalc()
{
    checkBoxSelected.release();
    checkBoxNotSelected.release();
    for (int i = 0; i < drawings.size(); ++i)
    {
        if (selections[i] == true)
        {
            drawings[i]->setMat(getCheckBoxSelected());
        }
        else
        {
            drawings[i]->setMat(getCheckBoxNotSelected());
        }
    }
    CompoundWidget::recalc();
}

cv::Mat CheckBoxes::getCheckBoxSelected()
{
    if (checkBoxSelected.empty())
    {
        checkBoxSelected = getCheckBoxNotSelected().clone();
        line(checkBoxSelected, Point(3, 6), Point(6, 11), getOutlineColor(), thickness, lineType);
        line(checkBoxSelected, Point(6, 11), Point(11, 1), getOutlineColor(), thickness, lineType);
    }
    return checkBoxSelected;
}

cv::Mat CheckBoxes::getCheckBoxNotSelected()
{
    if (checkBoxNotSelected.empty())
    {
        checkBoxNotSelected = Mat::zeros(14,14,CV_8UC4);
        rectangle(checkBoxNotSelected, Rect(2,2,10,10), getOutlineColor(), thickness, lineType);
    }
    return checkBoxNotSelected;
}

const char *CheckBoxes::getType() const
{
    return type;
}

shared_ptr<CheckBoxes> CheckBoxes::create(Layout &layoutVal, vector<string> checkBoxNames,
                                          CBUserSelection cbUserSelection, const Point &pos)
{
    assert(checkBoxNames.size());
    shared_ptr<CheckBoxes> checkBoxes(WidgetFactoryT<CheckBoxes>::newWidget(layoutVal, pos));

    for (int i = 0; i < checkBoxNames.size(); ++i)
    {
        checkBoxes->addCheckBox(checkBoxNames[i], cbUserSelection);
    }

    return checkBoxes;
}

void CheckBoxes::setChecked(int index, bool checked)
{
    if (index >= 0 && index < selections.size())
    {
        if (selections[index] != checked)
        {
            selections[index] = checked;
            setDirty();
        }
    }
}

bool CheckBoxes::isChecked(int index) const
{
    if (index >= 0 && index < selections.size())
    {
        return selections[index];
    }
    return false;
}

string CheckBoxes::getTextAt(int index) const
{
    if (index >= 0 && index < selections.size())
    {
        return frame->at<HorizontalLayout>(index)->at<Text>(1)->getMsg();
    }
    return "";
}

size_t CheckBoxes::size() const
{
   return selections.size();
}

}
