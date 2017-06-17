#ifndef SELECTIONBOX_H
#define SELECTIONBOX_H

#include <future>
#include "compoundwidget.h"
#include "vframe.h"

namespace canvascv
{

/**
 * @brief The SelectionBox class
 * Use a message box with any number of buttons on an OpenCV window
 */
class SelectionBox : public CompoundWidget
{
public:

    /**
     * @brief create a selection box widget
     * @param layout widgets are placed in layouts Canvas/VFrame/HFrame/...
     * @param selectionNames automatically create items with names of selectionNames
     * @param cbUserSelection a callback to invoke with index of pressed button
     * @param pos location in the Layout (if the layout supports Point locations)
     * @return a smart pointer copy of the object kept in the Layout
     * @code
        Canvas c(image.size());
        auto selectionBox = SelectionBox::create(c,
                                                 {
                                                 "Option1", // 0
                                                 "Option2", // 1
                                                 "Option3"  // 2
                                                 }, Point(100, 100));

        while(...)
        {
            c.redrawOn(...);
            imshow(...);
            key = waitKeyEx(...);
        }
     * @endcode
     */
    static std::shared_ptr<SelectionBox> create(Layout &layoutVal,
                                          std::vector<std::string> selectionNames,
                                          Widget::CBUserSelection cbUserSelection = Widget::CBUserSelection(),
                                          const cv::Point &pos = cv::Point(0,0));

    virtual const char *getType() const;

    static const char *type;

protected:
    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;

    SelectionBox(Layout &layoutVal, const cv::Point &pos);

private:
    std::shared_ptr<VFrame> frame;
};

}

/** @example example_selectionbox.cpp
 * This is an example of how to use the SelectionBox Widget.
 */

#endif // SELECTIONBOX_H
