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

HorizontalLayout::HorizontalLayout(Layout &layoutVal, const Point &pos)
    : LayoutBaseWidget(layoutVal, pos),
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

void HorizontalLayout::addWidget(const shared_ptr<Widget> &widget)
{
    Layout* prevLayout = widget->getLayout();
    if (prevLayout) prevLayout->rmvWidget(widget);
    widget->setLayout(*this);
    Point pos = location;
    if (flowAnchor & LEFT)
    {
        pos.x = rect.x + rect.width + spacing;
    }
    else if (flowAnchor & RIGHT)
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

bool HorizontalLayout::replaceTmpSharedPtr(const std::shared_ptr<Widget> &widget)
{
    auto i = find_if(horzWidgets.begin(),
                     horzWidgets.end(),
                     [widget](const shared_ptr<Widget> &item)->bool
    {
        return item.get() == widget.get();
    });
    if (i != horzWidgets.end())
    {
        i->reset();
        *i = widget;
        CompoundWidget::replaceTmpSharedPtr(widget);
        return true;
    }
    return false;
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
    if (flowAnchor & RIGHT) pos.x -= padding;
    else pos.x += padding;

    for (auto &widget : horzWidgets)
    {
        Anchor widgetLayoutAnchor = widget->getLayoutAnchor();
        if (widgetLayoutAnchor & BOTTOM)
        {
            // Align to the bottom - location.y is bottom-most position
            pos.y = location.y + padding + maxHeight - widget->getRect().height;
            if (flowAnchor & BOTTOM)
            {
                pos.y = location.y - padding;
            }
        }
        else if (widgetLayoutAnchor & CENTER)
        {
            pos.y = location.y + padding + maxHeight / 2. - widget->getRect().height / 2.;
            if (flowAnchor & BOTTOM)
            {
                pos.y = location.y - padding - maxHeight / 2. + widget->getRect().height / 2.;
            }
        }
        else
        {   // default is TOP
            pos.y = location.y + padding;
            if (flowAnchor & BOTTOM)
            {
                pos.y = location.y - padding - maxHeight + widget->getRect().height;
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
    }

    LayoutBaseWidget::recalc();
}

}
