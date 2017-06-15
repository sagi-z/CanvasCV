#ifndef LAYOUTBASE_H
#define LAYOUTBASE_H

#include "layout.h"

namespace canvascv
{

class LayoutBase : public Layout
{
public:
    LayoutBase();

    virtual bool addDirtyWidget(Widget *widget);
    virtual void rmvDirtyWidget(Widget *widget);

protected:
    virtual void updateDirtyWidgets();
    virtual bool isDuringUpdate() const;

private:
    virtual void setDirtyLayout() = 0;

    bool duringDirtyHandling;
    std::list<Widget*> dirtyWidgets;
};

}
#endif // LAYOUTBASE_H
