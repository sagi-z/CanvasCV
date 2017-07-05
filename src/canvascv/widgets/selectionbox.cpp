#include "widgetfactory.h"
#include "selectionbox.h"
#include "button.h"

using namespace std;
using namespace cv;

namespace canvascv
{

const char *SelectionBox::type = "SelectionBox";

/*
 *  -> VFrame
 *      -> Button (FLAT)
 *          ...
 */

SelectionBox::SelectionBox(const Point &pos)
    :CompoundWidget(pos)
{
    frame = VFrame::create(*this, pos);
    frame->setFrameRelief(RAISED);
    frame->setPadding(2);
    frame->setSpacing(3);
}

void SelectionBox::addButton(const string &txt)
{
    int i = frame->size();
    auto button = Button::create(*frame, txt);
    button->setFlowAnchor(LEFT);
    button->setStretchX(true);
    button->setFlatButton();
    button->notifyOnChange([this, i](Widget *, Widget::State state) {
        if (state == Widget::PRESS) {
            if (userCB) userCB(this, i);
        }
    });
}

void SelectionBox::recalcCompound()
{
}

const char *SelectionBox::getType() const
{
    return type;
}

shared_ptr<SelectionBox> SelectionBox::create(Layout &layoutVal, vector<string> selectionNames,
                                              CBUserSelection cbUserSelection, const Point &pos)
{
    assert(selectionNames.size());
    shared_ptr<SelectionBox> selectionBox(WidgetFactoryT<SelectionBox>::newWidget(layoutVal, pos));
    selectionBox->setUserCB(cbUserSelection);

    for (int i = 0; i < selectionNames.size(); ++i)
    {
        selectionBox->addButton(selectionNames[i]);
    }

    return selectionBox;
}

string SelectionBox::getTextAt(int index) const
{
    if (index >= 0 && index < size())
    {
        return frame->at<Button>(index)->getText();
    }
    return "";
}

size_t SelectionBox::size() const
{
    return frame->size();
}

void SelectionBox::setUserCB(Widget::CBUserSelection cbUserSelection)
{
    userCB  = cbUserSelection;
}

}
