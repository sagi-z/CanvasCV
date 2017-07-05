#include "canvascv/colors.h"
#include "compoundwidget.h"

#include <algorithm>

using namespace std;
using namespace cv;

namespace canvascv
{

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

void CompoundWidget::setSelectColor(const Scalar &value)
{
    Widget::setSelectColor(value);
    for (auto &widget : widgets)
    {
        widget->setSelectColor(value);
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

void CompoundWidget::setVisible(bool value)
{
    Widget::setVisible(value);
    for (auto &widget : widgets)
    {
        widget->setVisible(value);
    }
}

void CompoundWidget::recalc()
{
    updateDirtyWidgets();
    recalcCompound();
}

bool CompoundWidget::setDirtyLayout()
{
    return setDirty();
}

shared_ptr<Widget> CompoundWidget::rmvWidget(Widget *widget)
{
    shared_ptr<Widget> result;
    auto i = find_if(widgets.begin(),
                     widgets.end(),
                     [widget](const shared_ptr<Widget> &item)->bool
    {
        return item.get() == widget;
    });
    if (i != widgets.end())
    {
        result = *i;
        widgets.erase(i);
        rmvDirtyWidget(widget);
        if (widget == active.get())
        {
            active.reset();
        }
        setDirty();
    }
    return result;
}

shared_ptr<Widget> CompoundWidget::rmvWidget(const std::shared_ptr<Widget> &widget)
{
    return rmvWidget(widget.get());
}

/* TODO - write/read widgets to file for a designer app
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
*/

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

void CompoundWidget::broadcastChange(State status)
{
    if (active.get())
    {
        active->broadcastChange(status);
    }
}

// expect recalcRect() to call us ('rect' is already updated)
void CompoundWidget::recalcMinimalRect()
{
    minimalRect.x = rect.x;
    minimalRect.y = rect.y;
    int xMax = 0;
    int yMax = 0;
    for (auto &widget : widgets)
    {
        const Rect & subRect = widget->getMinimalRect();
        if (subRect.width && subRect.height)
        {
            xMax = max(xMax, subRect.x + subRect.width);
            yMax = max(yMax, subRect.y + subRect.height);
        }
    }
    if (xMax)
    {   // there was a calculation
        minimalRect.width = xMax - minimalRect.x;
        minimalRect.height = yMax - minimalRect.y;
    }
    if (forcedWidth && ! stretchX && ! stretchXToParent) minimalRect.width = forcedWidth;
    if (forcedHeight && ! stretchY && ! stretchYToParent) minimalRect.height = forcedHeight;
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

void CompoundWidget::recalcRect()
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
    if (forcedWidth) rect.width = forcedWidth;
    if (forcedHeight) rect.height = forcedHeight;
    recalcMinimalRect();
}

CompoundWidget::CompoundWidget(const Point &pos)
    :Widget(pos),
      fillBG(false)
{
    rect.x = location.x;
    rect.y = location.y;
    minimalRect.x = location.x;
    minimalRect.y = location.y;
}

bool CompoundWidget::isCompoundWidget() const
{
    return true;
}

const Rect CompoundWidget::getBoundaries() const
{
    Rect boundaries = rect;
    if (layout)
    {
        boundaries = getLayoutBoundaries();
    }
    if (forcedWidth) boundaries.width = forcedWidth;
    if (forcedHeight) boundaries.height = forcedHeight;
    return boundaries;
}

void CompoundWidget::renderOn(Mat &dst)
{
   if (visible)
   {
       if(fillBG) Widget::renderOn(dst); // just for frames
       for (auto &widget : widgets)
       {
           widget->renderOn(dst);
       }
   }
}

void CompoundWidget::drawFG(Mat &dst)
{
    (void) dst;
    // * All which was 'dirty' was already updated in out kids
    //  because:
    // 1. recalc of layouts called their update first/
    // 2. update of compounds called their update first.
    // * We don't have any FG of ourself.
}

const Rect &CompoundWidget::getRect()
{
    return rect;
}

const Rect &CompoundWidget::getMinimalRect()
{
    return minimalRect;
}

void CompoundWidget::addWidget(const shared_ptr<Widget> &widget)
{
    widget->rmvFromLayout();
    widget->setLayout(*this);
    widgets.push_back(widget);
    setDirty();
}

void CompoundWidget::translate(const Point &translation)
{
    if (translation.x != 0 || translation.y != 0)
    {
        location += translation;
        for (auto &widget : widgets)
        {
            widget->translate(translation);
        }
        setDirty();
    }
}

void CompoundWidget::doForAll(Widget::CBWidget cb, int recurseLevel, bool doOnSelf)
{
    if (cb)
    {
        if (doOnSelf) cb(this);
        if (recurseLevel)
        {
            for (auto &widget : widgets)
            {
                if (widget->isCompoundWidget())
                {
                    ((CompoundWidget*) widget.get())->doForAll(cb, recurseLevel - 1, true);
                }
                else
                {
                    cb(widget.get());
                }
            }
        }
    }
}

}
