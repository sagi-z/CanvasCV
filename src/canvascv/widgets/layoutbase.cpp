#include "layoutbase.h"
#include "widget.h"

namespace canvascv
{

LayoutBase::LayoutBase()
    : duringDirtyHandling(false)
{
}

LayoutBase::~LayoutBase()
{
}

bool LayoutBase::addDirtyWidget(Widget *widget)
{
    if (isDuringUpdate())
    {
        return false;
    }

    if (! setDirtyLayout())
    {
        return false;
    }

    dirtyWidgets.push_back(widget);
    return true;
}

void LayoutBase::rmvDirtyWidget(Widget *widget)
{
    auto pos = find(dirtyWidgets.begin(),
                    dirtyWidgets.end(),
                    widget);
    if (pos != dirtyWidgets.end()) dirtyWidgets.erase(pos);
}

void LayoutBase::updateDirtyWidgets()
{
    if (! duringDirtyHandling)
    {
        duringDirtyHandling = true;
        while (dirtyWidgets.size())
        {
            auto widget = dirtyWidgets.front();
            dirtyWidgets.pop_front();
            widget->update();
        }
        duringDirtyHandling = false;
    }
}

bool LayoutBase::hasDirtyWidgets()
{
   return ! dirtyWidgets.empty();
}

bool LayoutBase::isDuringUpdate() const
{
   return duringDirtyHandling;
}

}
