#include "layoutbase.h"
#include "widget.h"

namespace canvascv
{

LayoutBase::LayoutBase()
    : duringDirtyHandling(false)
{
}

void LayoutBase::addDirtyWidget(Widget *widget)
{
    if (! duringDirtyHandling)
    {
        dirtyWidgets.push_back(widget);
        setDirtyLayout();
    }
    else
    {
        // apply change immediatly
        widget->update();
    }
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

}
