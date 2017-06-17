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
    }
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

bool AutoLayout::getDrawFrame() const
{
    return getFillBG();
}

void AutoLayout::setDrawFrame(bool value)
{
    if (fillBG != value)
    {
        fillBG = value;
        setDirty();
    }
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
