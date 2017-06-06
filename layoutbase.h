#ifndef LAYOUTBASE_H
#define LAYOUTBASE_H

#include "layout.h"

namespace canvascv
{

class LayoutBase : public Layout
{
public:
    LayoutBase();

    virtual void addDirtyWidget(Widget *widget);
    virtual void rmvDirtyWidget(Widget *widget);

protected:
    virtual void upodateDirtyWidgets();

private:
    virtual void setDirtyLayout() = 0;

    bool duringDirtyHandling;
    std::list<Widget*> dirtyWidgets;
};

}
#endif // LAYOUTBASE_H
