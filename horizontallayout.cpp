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
    : CompoundWidget(pos),
      spacing(5),
      stretchX(false),
      stretchY(false)
{
}

shared_ptr<HorizontalLayout> HorizontalLayout::create(Layout &layout, const Point &pos)
{
    shared_ptr<HorizontalLayout> widget(WidgetFactoryT<HorizontalLayout>::newWidget(pos));
    layout.addWidget(widget);
    return widget;
}

const char *HorizontalLayout::getType() const
{
    return type;
}

// TODO
void HorizontalLayout::writeInternals(FileStorage &fs) const
{
}

// TODO
void HorizontalLayout::readInternals(const FileNode &node)
{
}

bool HorizontalLayout::getStretchY() const
{
    return stretchY;
}

void HorizontalLayout::setStretchY(bool value)
{
    if (stretchY != value)
    {
        stretchY = value;
        setDirty();
    }
}

bool HorizontalLayout::getStretchX() const
{
    return stretchX;
}

void HorizontalLayout::setStretchX(bool value)
{
    if (stretchX != value)
    {
        stretchX = value;
        setDirty();
    }
}

void HorizontalLayout::addWidget(const shared_ptr<Widget> &widget)
{
    Layout* prevLayout = widget->getLayout();
    if (prevLayout) prevLayout->rmvWidget(widget);
    widget->setLayout(*this);
    Point pos = location;
    if (layoutAnchor == TOP_LEFT)
    {
        pos.x = rect.x + rect.width + spacing;
    }
    else if (layoutAnchor == TOP_RIGHT)
    {
        pos.x = rect.x - spacing;
    }
    else
    {
        abort();
    }
    widget->setLocation(pos);
    horzWidgets.push_back(widget);
    CompoundWidget::addWidget(widget);
    setDirty();
}

void HorizontalLayout::rmvWidget(int i)
{
    if (i < horzWidgets.size())
    {
        shared_ptr<Widget> widget = horzWidgets[i];
        CompoundWidget::rmvWidget(widget);
        horzWidgets.erase(horzWidgets.begin() + i);
    }
    setDirty();
}

Widget *HorizontalLayout::at(int index)
{
   return horzWidgets.at(index).get();
}

bool HorizontalLayout::rmvWidget(const shared_ptr<Widget> &widget)
{
    if (CompoundWidget::rmvWidget(widget))
    {
        rmvDirtyWidget(widget.get());
        horzWidgets.erase(find(horzWidgets.begin(),
                               horzWidgets.end(),
                               widget));
        setDirty();
        return true;
    }
    return false;
}

void HorizontalLayout::recalc()
{
    upodateDirtyWidgets();
    int maxWidth = 0;
    int maxHeight = 0;
    for (auto &widget : horzWidgets)
    {
        const Rect &minRect = widget->getMinimalRect();
        maxWidth = max(maxWidth, minRect.width);
        maxHeight = max(maxHeight, minRect.height);
    }
    Point pos = location;
    for (auto &widget : horzWidgets)
    {
        Anchor widgetLayoutAnchor = widget->getLayoutAnchor();
        if (widgetLayoutAnchor & BOTTOM)
        {
            // Align to the bottom - location.y is bottom-most position
            pos.y = location.y + maxHeight - widget->getRect().height;
        }
        else if (widgetLayoutAnchor & CENTER)
        {
            pos.y = location.y + maxHeight / 2. - widget->getRect().height / 2.;
        }
        else
        {   // default is TOP, pos.y unchanged
            pos.y = location.y;
        }
        widget->setLocation(pos);
        if (stretchX) widget->stretchWidth(maxWidth);
        if (stretchY) widget->stretchHeight(maxHeight);

        // Next col is either to the left or right
        if (flowAnchor & RIGHT)
        {
            pos.x -= (widget->getRect().width + spacing);
            if (pos.x < 0) pos.x = 0; // try to avoid opencv aborts
        }
        else // default is LEFT
        {
            pos.x += (widget->getRect().width + spacing);
        }
    }

    CompoundWidget::recalc();
}

bool HorizontalLayout::isDuringUpdate() const
{
   return delayedUpdate == false;
}

const Rect HorizontalLayout::getBoundaries() const
{
    Rect boundaries = rect;
    if (layout)
    {
        boundaries = layout->getBoundaries();
    }
    if (forcedWidth) boundaries.width = forcedWidth;
    if (forcedHeight) boundaries.height = forcedHeight;
    return boundaries;
}

void HorizontalLayout::setDirtyLayout()
{
    setDirty();
}

}
