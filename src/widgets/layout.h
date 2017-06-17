#ifndef LAYOUT_H
#define LAYOUT_H

#include <opencv2/core.hpp>
#include <memory>
#include "widget.h"

namespace canvascv
{

/**
 * @brief The Layout class
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
     * @param widget will be removed from this Layout
     * @return true if widget was a part of this Layout and false otherwise
     * @note
     * Widgets must be in layouts to be displayed correctly.
     */
    virtual bool rmvWidget(const std::shared_ptr<Widget> &widget) = 0;

    friend class WidgetFactory;
    friend class Widget;

    virtual bool rmvWidget(Widget* widget) = 0;
    virtual const cv::Rect getBoundaries() const = 0;
    virtual bool addDirtyWidget(Widget *widget) = 0;
    virtual void rmvDirtyWidget(Widget *widget) = 0;

    // takes the shared_ptr added in the widget CTOR and replace it
    // with the fully constructed one (doing nothing else)
    virtual bool replaceTmpSharedPtr(const std::shared_ptr<Widget> &widget) = 0;

    virtual void updateDirtyWidgets() = 0;
};

}

#endif // LAYOUT_H
