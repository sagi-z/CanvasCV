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

VerticalLayout::VerticalLayout(const Point &pos)
    : AutoLayout(pos),
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

void VerticalLayout::recalcCompound()
{
    if (wrap)
    {
        Rect boundaries = getLayoutBoundaries();
        forcedHeight = boundaries.height - location.y;
    }
    int maxWidthWrap = 0;
    int maxWidth = 0;
    int maxHeight = 0;
    for (auto &widget : widgets)
    {
        const Rect &minRect = widget->getMinimalRect();
        maxWidth = max(maxWidth, minRect.width);
        maxHeight = max(maxHeight, minRect.height);
    }
    if (forcedWidth) maxWidth = forcedWidth;
    if (forcedHeight) maxHeight = forcedHeight;

    Point pos = location;
    if (flowAnchor & BOTTOM) pos.y -= padding;
    else pos.y += padding;

    int posYStart = pos.y;
    int currentPosX = location.x;
    for (auto &widget : widgets)
    {
        if (wrap && forcedHeight)
        {
            if (abs(pos.y - posYStart) + padding + widget->getRect().height > forcedHeight)
            {
                // update X
                if (flowAnchor & RIGHT) currentPosX -= maxWidthWrap - spacing;
                else currentPosX += maxWidthWrap + spacing;

                // reset Y
                pos.y = posYStart;

                maxWidthWrap = 0;
            }
        }

        Anchor widgetLayoutAnchor = widget->getLayoutAnchor();
        if (widgetLayoutAnchor & RIGHT)
        {
            // Align to the right - currentPosX is rightmost position
            pos.x = currentPosX + padding + maxWidth - widget->getRect().width;
            if (flowAnchor & RIGHT)
            {
                pos.x = currentPosX - padding;
            }
        }
        else if (widgetLayoutAnchor & CENTER)
        {
            pos.x = currentPosX + padding + maxWidth / 2. - widget->getRect().width / 2.;
            if (flowAnchor & RIGHT)
            {
                pos.x = currentPosX - padding - maxWidth / 2. + widget->getRect().width / 2.;
            }
        }
        else
        {   // default is LEFT
            pos.x = currentPosX + padding;
            if (flowAnchor & RIGHT)
            {
                pos.x = currentPosX - padding - maxWidth + widget->getRect().width;
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
        widget->update();

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
        maxWidthWrap = max(maxWidthWrap, widget->getRect().width);
    }

    AutoLayout::recalcAndAllocate();
}

}
