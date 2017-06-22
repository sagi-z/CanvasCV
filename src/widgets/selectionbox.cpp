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

SelectionBox::SelectionBox(Layout &layoutVal, const Point &pos)
    :CompoundWidget(layoutVal, pos)
{
    frame = VFrame::create(*this, pos);
    frame->setFrameRelief(RAISED);
    frame->setPadding(2);
    frame->setSpacing(3);
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

    for (int i = 0; i < selectionNames.size(); ++i)
    {
        auto button = Button::create(*selectionBox->frame, selectionNames[i]);
        button->setFlowAnchor(LEFT);
        button->setStretchX(true);
        button->setFlatButton();
        button->notifyOnChange([selectionBox, i, cbUserSelection](Widget *, Widget::State state) {
            if (state == Widget::PRESS) {
                if (cbUserSelection) cbUserSelection(selectionBox.get(), i);
            }
        });
    }

    return selectionBox;
}

string SelectionBox::getTextAt(int index) const
{
    if (index >= 0 && index < frame->size())
    {
        return frame->at<Button>(index)->getMsg();
    }
    return "";
}

size_t SelectionBox::size() const
{
    return frame->size();
}

}
