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

    // A Layout is also a widget and its 'recalc' method will update the dirty widgets
    virtual void addDirtyWidget(Widget *widget) = 0;

    virtual cv::Size getAllowedSize() const = 0;

    virtual void addWidget(const std::shared_ptr<Widget> &widget) = 0;
    virtual bool rmvWidget(const std::shared_ptr<Widget> &widget) = 0;
};

}

#endif // LAYOUT_H
