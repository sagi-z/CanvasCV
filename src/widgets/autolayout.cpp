#include "autolayout.h"

using namespace cv;
using namespace std;

namespace canvascv
{

AutoLayout::AutoLayout(Layout &layoutVal, const Point &pos)
    : CompoundWidget(layoutVal, pos),
      padding(2)
{
    fillBG = false;
}

bool AutoLayout::isDuringUpdate() const
{
    return delayedUpdate == false;
}

void AutoLayout::recalc()
{
    CompoundWidget::recalc();
    if (fillBG)
    {
        if (padding)
        {
            rect.x -= padding;
            rect.y -= padding;
            rect.width += padding*2;
            rect.height += padding*2;
        }
        allocateBG(rect.size());
//        callDrawFG();
    }

    /* This solution is currently replaced with Canvas ROI clipping
    Point rectTL = rect.tl();
    Rect parentBoundaries = getLayoutBoundaries();
    Point parentBoundariesTL = parentBoundaries.tl();
    Point parentBoundariesBR = parentBoundaries.br();
    if (rectTL.x < parentBoundariesTL.x) rectTL.x = parentBoundariesTL.x;
    if (rectTL.y < parentBoundariesTL.y) rectTL.y = parentBoundariesTL.y;
    if (rectTL.x + rect.width > parentBoundariesBR.x) rectTL.x = parentBoundariesBR.x - rect.width;
    if (rectTL.y + rect.height > parentBoundariesBR.y) rectTL.y = parentBoundariesBR.y - rect.height;
    if (rectTL.y != rect.y || rectTL.x != rect.x)
    {
        translate(rectTL - rect.tl());
    }
    */
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

bool AutoLayout::rmvWidget(const std::shared_ptr<Widget> &widget)
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
