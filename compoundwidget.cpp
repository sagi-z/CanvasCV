#include "colors.h"
#include "compoundwidget.h"

#include <algorithm>

namespace canvascv
{

void CompoundWidget::draw(cv::Mat &canvas)
{
    if (visible)
    {
        for (auto &widget : widgets)
        {
            if (widget->getVisible())
            {
                widget->draw(canvas);
            }
        }
    }
}

void CompoundWidget::setOutlineColor(const cv::Scalar &value)
{
    Widget::setOutlineColor(value);
    for (auto &widget : widgets)
    {
        widget->setOutlineColor(value);
    }
}

void CompoundWidget::setFillColor(const cv::Scalar &value)
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

bool CompoundWidget::rmvWidget(std::shared_ptr<Widget> &widget)
{
    std::list<std::shared_ptr<Widget>>::iterator i = find(widgets.begin(),widgets.end(),widget);
    if (i != widgets.end())
    {
        widgets.erase(i);
        return true;
    }
    return false;
}

void CompoundWidget::writeInternals(cv::FileStorage &fs) const
{
    Widget::writeInternals(fs);
    fs << "widgets" << "[";
    for (auto &widget : widgets)
    {
        fs << *widget;
    }
    fs << "]";
}

void CompoundWidget::readInternals(const cv::FileNode &node)
{
    Widget::readInternals(node);
    active.reset();
    widgets.clear();
    cv::FileNode n = node["widgets"];
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

bool CompoundWidget::isAtPos(const cv::Point &pos)
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

void CompoundWidget::broadcastChange(canvascv::Widget::State status)
{
    if (active.get())
    {
        active->broadcastChange(status);
    }
}

}
