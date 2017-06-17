#include "verticallayout.h"
#include "widgetfactory.h"

using namespace std;
using namespace cv;

namespace canvascv
{

const char *VerticalLayout::type = "VerticalLayout";

int VerticalLayout::getSpacing() const
{
    return spacing;
}

void VerticalLayout::setSpacing(int value)
{
    spacing = value;
    setDirty();
}

VerticalLayout::VerticalLayout(Layout &layoutVal, const Point &pos)
    : AutoLayout(layoutVal, pos),
      spacing(5)
{
}

shared_ptr<VerticalLayout> VerticalLayout::create(Layout &layout, const Point &pos)
{
    shared_ptr<VerticalLayout> widget(WidgetFactoryT<VerticalLayout>::newWidget(layout, pos));
    return widget;
}

const char *VerticalLayout::getType() const
{
    return type;
}

/* TODO - write/read widgets to file for a designer app
void VerticalLayout::writeInternals(FileStorage &fs) const
{
}

void VerticalLayout::readInternals(const FileNode &node)
{
}
*/

void VerticalLayout::recalc()
{
    updateDirtyWidgets();
    int maxWidth = 0;
    int maxHeight = 0;
    for (auto &widget : widgets)
    {
        const Rect &minRect = widget->getMinimalRect();
        maxWidth = max(maxWidth, minRect.width);
        maxHeight = max(maxHeight, minRect.height);
    }

    Point pos = location;
    if (flowAnchor & BOTTOM) pos.y -= padding;
    else pos.y += padding;

    for (auto &widget : widgets)
    {
        Anchor widgetLayoutAnchor = widget->getLayoutAnchor();
        if (widgetLayoutAnchor & RIGHT)
        {
            // Align to the right - location.x is rightmost position
            pos.x = location.x + padding + maxWidth - widget->getRect().width;
            if (flowAnchor & RIGHT)
            {
                pos.x = location.x - padding;
            }
        }
        else if (widgetLayoutAnchor & CENTER)
        {
            pos.x = location.x + padding + maxWidth / 2. - widget->getRect().width / 2.;
            if (flowAnchor & RIGHT)
            {
                pos.x = location.x - padding - maxWidth / 2. + widget->getRect().width / 2.;
            }
        }
        else
        {   // default is LEFT
            pos.x = location.x + padding;
            if (flowAnchor & RIGHT)
            {
                pos.x = location.x - padding - maxWidth + widget->getRect().width;
            }
        }

        if (flowAnchor & BOTTOM)
        {
            pos.y -= widget->getRect().height;
            if (pos.y < 0) pos.y = 0; // try to avoid opencv aborts
        }
        else // default is TOP
        {
            // do nothing
        }
        widget->setLocation(pos);
        if (widget->getStretchX()) widget->stretchWidth(maxWidth);
        if (widget->getStretchY()) widget->stretchHeight(maxHeight);

        // prepare for the next iteration
        if (flowAnchor & BOTTOM)
        {
            pos.y -=  spacing;
            if (pos.y < 0) pos.y = 0; // try to avoid opencv aborts
        }
        else // default is TOP
        {
            pos.y += widget->getRect().height + spacing;
        }
    }

    AutoLayout::recalc();
}

}
