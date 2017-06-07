#include "colors.h"
#include "compoundwidget.h"

#include <algorithm>

using namespace std;
using namespace cv;

namespace canvascv
{

void CompoundWidget::draw(Mat &dst)
{
    if (visible)
    {
        for (auto &widget : widgets)
        {
            if (widget->getVisible())
            {
                widget->draw(dst);
            }
        }
    }
}

void CompoundWidget::setOutlineColor(const Scalar &value)
{
    Widget::setOutlineColor(value);
    for (auto &widget : widgets)
    {
        widget->setOutlineColor(value);
    }
}

void CompoundWidget::setFillColor(const Scalar &value)
{
    Widget::setFillColor(value);
    for (auto &widget : widgets)
    {
        widget->setFillColor(value);
    }
}

void CompoundWidget::setThickness(int value)
{
    Widget::setThickness(value);
    for (auto &widget : widgets)
    {
        widget->setThickness(value);
    }
}

void CompoundWidget::setLineType(int value)
{
    Widget::setLineType(value);
    for (auto &widget : widgets)
    {
        widget->setLineType(value);
    }
}

void CompoundWidget::setLocked(bool value)
{
    Widget::setLocked(value);
    for (auto &widget : widgets)
    {
        widget->setLocked(value);
    }
}

void CompoundWidget::setVisible(bool value)
{
    Widget::setVisible(value);
    for (auto &widget : widgets)
    {
        widget->setVisible(value);
    }
}

bool CompoundWidget::rmvWidget(Widget *widget)
{
    list<shared_ptr<Widget>>::iterator i = find_if(widgets.begin(),
                                                   widgets.end(),
                                                   [widget](const shared_ptr<Widget> &item)->bool
    {
        return item.get() == widget;
    });
    if (i != widgets.end())
    {
        widgets.erase(i);
        return true;
    }
    return false;
}

bool CompoundWidget::rmvWidget(const std::shared_ptr<Widget> &widget)
{
    list<shared_ptr<Widget>>::iterator i = find_if(widgets.begin(),
                                                   widgets.end(),
                                                   [widget](const shared_ptr<Widget> &item)->bool
    {
        return item.get() == widget.get();
    });
    if (i != widgets.end())
    {
        widgets.erase(i);
        CompoundWidget::recalc();
        return true;
    }
    return false;
}

void CompoundWidget::writeInternals(FileStorage &fs) const
{
    Widget::writeInternals(fs);
    fs << "widgets" << "[";
    for (auto &widget : widgets)
    {
        fs << *widget;
    }
    fs << "]";
}

void CompoundWidget::readInternals(const FileNode &node)
{
    Widget::readInternals(node);
    active.reset();
    widgets.clear();
    FileNode n = node["widgets"];
    FileNodeIterator it = n.begin(), it_end = n.end();
    std::list<Widget*> widgetsTmp;
    for (; it != it_end; )
    { // ++it is done automatically by "it >> widget"
        Widget *widget = 0;
        it >> widget;
        assert(widget != 0);
        widgets.push_back(std::shared_ptr<Widget>(widget));
        widgetsTmp.push_back(widget);
    }
    std::list<Widget*>::const_iterator i = widgetsTmp.begin();
    reloadPointers(i);
}

bool CompoundWidget::isAtPos(const Point &pos)
{
    if (active.get())
    {
        if (active->isAtPos(pos))
        {
            return true;
        }
        else
        {
            active->broadcastChange(LEAVE);
            active.reset();
        }
    }
    for (auto &widget : widgets)
    {
        if (widget->isAtPos(pos))
        {
            active = widget;
            return true;
        }
    }
    return false;
}

void CompoundWidget::broadcastChange(Widget::State status)
{
    if (active.get())
    {
        active->broadcastChange(status);
    }
}

void CompoundWidget::layoutResized(const Rect &boundaries)
{
    for (auto &widget : widgets)
    {
        widget->layoutResized(boundaries);
    }
}

const string &CompoundWidget::getStatusMsg() const
{
   if (active.get())
   {
       return active->getStatusMsg();
   }
   else
   {
       return Widget::getStatusMsg();
   }
}

void CompoundWidget::recalc()
{
    int xMin = INT_MAX;
    int yMin = INT_MAX;
    int xMax = 0;
    int yMax = 0;
    for (auto &widget : widgets)
    {
        const Rect & subRect = widget->getRect();
        if (subRect.width && subRect.height)
        {
            xMin = min(xMin, subRect.x);
            yMin = min(yMin, subRect.y);
            xMax = max(xMax, subRect.x + subRect.width);
            yMax = max(yMax, subRect.y + subRect.height);
        }
    }
    if (xMax)
    {   // there was a calculation
        rect.x = xMin;
        rect.y = yMin;
        rect.width = xMax - xMin;
        rect.height = yMax - yMin;
    }
}

CompoundWidget::CompoundWidget(const Point &pos)
    :Widget(pos)
{
    rect.x = location.x;
    rect.y = location.y;
}

const Rect &CompoundWidget::getRect()
{
    return rect;
}

const Rect &CompoundWidget::getMinimalRect()
{
    return rect;
}

void CompoundWidget::addWidget(const shared_ptr<Widget> &widget)
{
    rmvWidget(widget);
    widgets.push_back(widget);
    CompoundWidget::recalc();
}

void CompoundWidget::translate(const Point &translation)
{
    location += translation;
    for (auto &widget : widgets)
    {
        widget->translate(translation);
    }
    setDirty();
}

}
