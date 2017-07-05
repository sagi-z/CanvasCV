#include "autolayout.h"

using namespace cv;
using namespace std;

namespace canvascv
{

AutoLayout::AutoLayout(const Point &pos)
    : CompoundWidget(pos),
      padding(2),
      wrap(false),
      maxWidth(0),
      maxHeight(0)
{
    fillBG = false;
}

void AutoLayout::recalcAndAllocate()
{
    recalcRect();
    if (fillBG)
    {
        if (padding)
        {
            // restore the padding that recalcRect() removed
            rect.x -= padding;
            rect.y -= padding;
            rect.width += padding*2;
            rect.height += padding*2;
        }
        allocateBG(rect.size());
    }
}

const Rect AutoLayout::getBoundaries() const
{
    if (padding)
    {
        Rect boundaries = CompoundWidget::getBoundaries();
        boundaries.x += padding;
        boundaries.y += padding;
        boundaries.width -= 2 * padding;
        boundaries.height -= 2 * padding;
        return boundaries;
    }
    else
    {
        return CompoundWidget::getBoundaries();
    }
}

bool AutoLayout::getWrap() const
{
    return wrap;
}

void AutoLayout::setWrap(bool value)
{
    if (wrap != value)
    {
        wrap = value;
        if (wrap)
        {
            setStretchX(false);
            setStretchY(false);
            setStretchXToParent(false);
            setStretchYToParent(false);
        }
        setDirty();
    }
}

int AutoLayout::getMaxWidgetWidth()
{
    return maxWidth;
}

int AutoLayout::getMaxWidgetHeight()
{
    return maxHeight;
}

int AutoLayout::getPadding() const
{
    return padding;
}

void AutoLayout::setPadding(int value)
{
    if (padding != value)
    {
        padding = value;
        setDirty();
    }
}

void AutoLayout::addWidget(const std::shared_ptr<Widget> &widget)
{
    CompoundWidget::addWidget(widget);
}

shared_ptr<Widget> AutoLayout::rmvWidget(const std::shared_ptr<Widget> &widget)
{
   return CompoundWidget::rmvWidget(widget);
}

void AutoLayout::rmvWidget(int i)
{
    if (i < widgets.size())
    {
        Widget* pWidget = at(i);
        CompoundWidget::rmvWidget(pWidget);
        setDirty();
    }
}

size_t AutoLayout::size() const
{
   return widgets.size();
}

template<>
Widget *AutoLayout::at(int index)
{
    Widget *pWidget = nullptr;
    if (index < widgets.size())
    {
        auto i = widgets.begin();
        std::advance(i, index);
        pWidget = i->get();
    }
    return pWidget;
}

}
