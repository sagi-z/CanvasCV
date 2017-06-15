#ifndef LAYOUT_H
#define LAYOUT_H

#include <opencv2/core.hpp>
#include <memory>
#include "widget.h"

namespace canvascv
{

class Layout
{
public:
    virtual ~Layout();

    virtual bool addDirtyWidget(Widget *widget) = 0;
    virtual void rmvDirtyWidget(Widget *widget) = 0;

    virtual const cv::Rect getBoundaries() const = 0;

    virtual void addWidget(const std::shared_ptr<Widget> &widget) = 0;
    virtual bool rmvWidget(const std::shared_ptr<Widget> &widget) = 0;

protected:
    friend class WidgetFactory;

    // takes the shared_ptr added in the widget CTOR and replace it
    // with the fully constructed one (doing nothing else)
    virtual bool replaceTmpSharedPtr(const std::shared_ptr<Widget> &widget) = 0;

    virtual void updateDirtyWidgets() = 0;
};

}

#endif // LAYOUT_H
