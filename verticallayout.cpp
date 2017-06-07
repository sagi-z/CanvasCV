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
    : CompoundWidget(pos),
      spacing(5)
{
}

shared_ptr<VerticalLayout> VerticalLayout::create(Layout &layout, const Point &pos)
{
    shared_ptr<VerticalLayout> widget(WidgetFactoryT<VerticalLayout>::newWidget(pos));
    layout.addWidget(widget);
    return widget;
}

const char *VerticalLayout::getType() const
{
    return type;
}

// TODO
void VerticalLayout::writeInternals(FileStorage &fs) const
{
}

// TODO
void VerticalLayout::readInternals(const FileNode &node)
{
}

void VerticalLayout::addWidget(const shared_ptr<Widget> &widget)
{
    Layout* prevLayout = widget->getLayout();
    if (prevLayout) prevLayout->rmvWidget(widget);
    widget->setLayout(*this);
    Point pos = location;
    if (layoutAnchor == TOP_LEFT)
    {
        pos.y = rect.y + rect.height + spacing;
    }
    else if (layoutAnchor == BOTTOM_LEFT)
    {
        pos.y = rect.y - spacing;
    }
    else
    {
        abort();
    }
    widget->setLocation(pos);
    vertWidgets.push_back(widget);
    CompoundWidget::addWidget(widget);
    setDirty();
}

void VerticalLayout::rmvWidget(int i)
{
    if (i < vertWidgets.size())
    {
        shared_ptr<Widget> widget = vertWidgets[i];
        CompoundWidget::rmvWidget(widget);
        vertWidgets.erase(vertWidgets.begin() + i);
    }
    setDirty();
}

Widget *VerticalLayout::at(int index)
{
   return vertWidgets.at(index).get();
}

bool VerticalLayout::rmvWidget(const shared_ptr<Widget> &widget)
{
    if (CompoundWidget::rmvWidget(widget))
    {
        rmvDirtyWidget(widget.get());
        vertWidgets.erase(find(vertWidgets.begin(),
                               vertWidgets.end(),
                               widget));
        setDirty();
        return true;
    }
    return false;
}

void VerticalLayout::recalc()
{
    upodateDirtyWidgets();
    int maxWidth = 0;
    int maxHeight = 0;
    for (auto &widget : vertWidgets)
    {
        const Rect &minRect = widget->getMinimalRect();
        maxWidth = max(maxWidth, minRect.width);
        maxHeight = max(maxHeight, minRect.height);
    }
    Point pos = location;
    for (auto &widget : vertWidgets)
    {
        Anchor widgetLayoutAnchor = widget->getLayoutAnchor();
        if (widgetLayoutAnchor & RIGHT)
        {
            // Align to the right - location.x is rightmost position
            pos.x = location.x + maxWidth - widget->getRect().width;
        }
        else if (widgetLayoutAnchor & CENTER)
        {
            pos.x = location.x + maxWidth / 2. - widget->getRect().width / 2.;
        }
        else
        {   // default is LEFT, pos.x unchanged
            pos.x = location.x;
        }
        widget->setLocation(pos);
        if (widget->getStretchX()) widget->stretchWidth(maxWidth);
        if (widget->getStretchY()) widget->stretchHeight(maxHeight);

        // Next row is either below us or above us
        if (flowAnchor & BOTTOM)
        {
            pos.y -= (widget->getRect().height + spacing);
            if (pos.y < 0) pos.y = 0; // try to avoid opencv aborts
        }
        else // default is TOP
        {
            pos.y += (widget->getRect().height + spacing);
        }
    }

    CompoundWidget::recalc();
}

bool VerticalLayout::isDuringUpdate() const
{
   return delayedUpdate == false;
}

const Rect VerticalLayout::getBoundaries() const
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

void VerticalLayout::setDirtyLayout()
{
    setDirty();
}

}
