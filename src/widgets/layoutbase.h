#ifndef LAYOUTBASE_H
#define LAYOUTBASE_H

#include "layout.h"

namespace canvascv
{

/**
 * @brief The LayoutBase class
 * Common base class for all Layout classes
 */
class LayoutBase : public Layout
{
protected:
    LayoutBase();
    virtual ~LayoutBase();

    virtual bool addDirtyWidget(Widget *widget);
    virtual void rmvDirtyWidget(Widget *widget);
    virtual void updateDirtyWidgets();
    virtual bool isDuringUpdate() const;

private:
    virtual bool setDirtyLayout() = 0;

    bool duringDirtyHandling;
    std::list<Widget*> dirtyWidgets;
};

}
#endif // LAYOUTBASE_H
