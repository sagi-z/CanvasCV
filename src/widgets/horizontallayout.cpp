#include "horizontallayout.h"
#include "widgetfactory.h"

using namespace std;
using namespace cv;

namespace canvascv
{

const char *HorizontalLayout::type = "HorizontalLayout";

int HorizontalLayout::getSpacing() const
{
    return spacing;
}

void HorizontalLayout::setSpacing(int value)
{
    spacing = value;
    setDirty();
}

HorizontalLayout::HorizontalLayout(const Point &pos)
    : AutoLayout(pos),
      spacing(5)
{
}

shared_ptr<HorizontalLayout> HorizontalLayout::create(Layout &layout, const Point &pos)
{
    shared_ptr<HorizontalLayout> widget(WidgetFactoryT<HorizontalLayout>::newWidget(layout, pos));
    return widget;
}

const char *HorizontalLayout::getType() const
{
    return type;
}

/* TODO - write/read widgets to file for a designer app
void HorizontalLayout::writeInternals(FileStorage &fs) const
{
}

void HorizontalLayout::readInternals(const FileNode &node)
{
}
*/

void HorizontalLayout::recalcCompound()
{
    if (wrap)
    {
        Rect boundaries = getLayoutBoundaries();
        forcedWidth = boundaries.width - location.x;
    }
    int maxHeightWrap = 0;
    int maxWidth = 0;
    int maxHeight = 0;
    for (auto &widget : widgets)
    {
        const Rect &minRect = widget->getMinimalRect();
        maxWidth = max(maxWidth, minRect.width);
        maxHeight = max(maxHeight, minRect.height);
    }

    Point pos = location;
    if (flowAnchor & RIGHT) pos.x -= padding;
    else pos.x += padding;

    int posXStart = pos.x;
    int currentPosY = location.y;
    for (auto &widget : widgets)
    {
        if (wrap && forcedWidth)
        {
            if (abs(pos.x - posXStart) + padding + widget->getRect().width > forcedWidth)
            {
                // update Y
                if (flowAnchor & BOTTOM) currentPosY -= maxHeightWrap - spacing ;
                else currentPosY += maxHeightWrap + spacing;

                // reset X
                pos.x = posXStart;

                maxHeightWrap = 0;
            }
        }

        Anchor widgetLayoutAnchor = widget->getLayoutAnchor();
        if (widgetLayoutAnchor & BOTTOM)
        {
            // Align to the bottom - currentPosY is bottom-most position
            pos.y = currentPosY + padding + maxHeight - widget->getRect().height;
            if (flowAnchor & BOTTOM)
            {
                pos.y = currentPosY - padding;
            }
        }
        else if (widgetLayoutAnchor & CENTER)
        {
            pos.y = (int) currentPosY + padding + maxHeight / 2. - widget->getRect().height / 2.;
            if (flowAnchor & BOTTOM)
            {
                pos.y = (int) currentPosY - padding - maxHeight / 2. + widget->getRect().height / 2.;
            }
        }
        else
        {   // default is TOP
            pos.y = currentPosY + padding;
            if (flowAnchor & BOTTOM)
            {
                pos.y = currentPosY - padding - maxHeight + widget->getRect().height;
            }
        }

        if (flowAnchor & RIGHT)
        {   // Next col is to the left of pos, so start at an offset there
            pos.x -= widget->getRect().width;
            if (pos.x < 0) pos.x = 0; // try to avoid opencv aborts
        }
        else // default is LEFT
        {
            // nothing to do
        }
        widget->setLocation(pos);
        if (widget->getStretchX()) widget->stretchWidth(maxWidth);
        if (widget->getStretchY()) widget->stretchHeight(maxHeight);
        widget->update();

        // prepare for the next iteration
        if (flowAnchor & RIGHT)
        {
            pos.x -= spacing;
            if (pos.x < 0) pos.x = 0; // try to avoid opencv aborts
        }
        else // default is LEFT
        {
            pos.x += widget->getRect().width + spacing;
        }
        maxHeightWrap = max(maxHeightWrap, widget->getRect().height);
    }

    AutoLayout::recalcAndAllocate();
}

}
