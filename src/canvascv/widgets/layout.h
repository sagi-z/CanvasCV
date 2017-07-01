#ifndef LAYOUT_H
#define LAYOUT_H

#include <opencv2/core.hpp>
#include <memory>
#include "widget.h"

namespace canvascv
{

/**
 * @brief The Layout class
 * 
 * This is the layout interface class. All widgets must go into a Layout.
 * All Layouts are also widgets except for the Canvas class.
 */
class Layout
{
protected:
    /// adds the widget to this Layout after removing it from it's previous layout
    virtual void addWidget(const std::shared_ptr<Widget> &widget) = 0;

    /**
     * @brief rmvWidget
     * 
     * @param widget will be removed from this Layout
     * @return filled shared_ptr to removed widget or empty if not found
     * @note
     * Widgets must be in layouts to be displayed correctly.
     */
    virtual std::shared_ptr<Widget> rmvWidget(const std::shared_ptr<Widget> &widget) = 0;

    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;
    friend class Widget;

    virtual std::shared_ptr<Widget> rmvWidget(Widget* widget) = 0;
    virtual const cv::Rect getBoundaries() const = 0;
    virtual bool addDirtyWidget(Widget *widget) = 0;
    virtual void rmvDirtyWidget(Widget *widget) = 0;

    virtual void updateDirtyWidgets() = 0;
};

}

#endif // LAYOUT_H
