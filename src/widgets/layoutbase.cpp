#include "layoutbase.h"
#include "widget.h"

namespace canvascv
{

LayoutBase::LayoutBase()
    : duringDirtyHandling(false)
{
}

bool LayoutBase::addDirtyWidget(Widget *widget)
{
    if (isDuringUpdate()) return false;

    dirtyWidgets.push_back(widget);
    setDirtyLayout();
    return true;
}

void LayoutBase::rmvDirtyWidget(Widget *widget)
{
    auto pos = find(dirtyWidgets.begin(),
                    dirtyWidgets.end(),
                    widget);
    if (pos != dirtyWidgets.end()) dirtyWidgets.erase(pos);
}

void LayoutBase::upodateDirtyWidgets()
{
    duringDirtyHandling = true;
    while (dirtyWidgets.size())
    {
        dirtyWidgets.front()->update();
        dirtyWidgets.pop_front();
    }
    duringDirtyHandling = false;
}

bool LayoutBase::isDuringUpdate() const
{
   return duringDirtyHandling;
}

}
