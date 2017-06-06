#include "verticallayout.h"
#include "canvas.h"

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
      spacing(5),
      stretchX(false),
      stretchY(false)
{
}

shared_ptr<VerticalLayout> VerticalLayout::newVerticalLayout(Canvas &c, Point pos)
{
    shared_ptr<VerticalLayout> widget = c.createWidget<VerticalLayout>(pos);
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

bool VerticalLayout::getStretchY() const
{
    return stretchY;
}

void VerticalLayout::setStretchY(bool value)
{
    if (stretchY != value)
    {
        stretchY = value;
        setDirty();
    }
}

bool VerticalLayout::getStretchX() const
{
    return stretchX;
}

void VerticalLayout::setStretchX(bool value)
{
    if (stretchX != value)
    {
        stretchX = value;
        setDirty();
    }
}

void VerticalLayout::addWidget(const shared_ptr<Widget> &widget)
{
    Layout* prevLayout = widget->getLayout();
    if (prevLayout) prevLayout->rmvWidget(widget);
    widget->setLayout(*this);
    Point pos = leftPos;
    if (anchor == TOP_LEFT)
    {
        pos.y = rect.y + rect.height + spacing;
    }
    else if (anchor == BOTTOM_LEFT)
    {
        pos.y = rect.y - spacing;
    }
    else
    {
        abort();
    }
    widget->setLeftPos(pos);
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
    Point pos = leftPos;
    for (auto &widget : vertWidgets)
    {
        widget->setLeftPos(pos);
        if (stretchX) widget->stretchWidth(maxWidth);
        if (stretchY) widget->stretchHeight(maxHeight);
        if (anchor == TOP_LEFT)
        {
            pos.y += (widget->getRect().height + spacing);
        }
        else if (anchor == BOTTOM_LEFT)
        {
            pos.y -= (widget->getRect().height + spacing);
        }
        else
        {
            abort();
        }
    }

    CompoundWidget::recalc();
}

Size VerticalLayout::getAllowedSize() const
{
    if (layout)
    {
        return layout->getAllowedSize();
    }
    else
    {
        return rect.size();
    }
}

void VerticalLayout::setDirtyLayout()
{
    setDirty();
}

}
