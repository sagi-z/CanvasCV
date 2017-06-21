#ifndef RADIOBUTTONS_H
#define RADIOBUTTONS_H

#include <future>
#include "compoundwidget.h"
#include "vframe.h"

namespace canvascv
{

class MatWidget;

/**
 * @brief The RadioButtons class
 * Use a radio buttons to get a user selection for multiple options
 */
class RadioButtons : public CompoundWidget
{
public:

    /**
     * @brief create a radio buttons widget
     * @param layout widgets are placed in layouts Canvas/VFrame/HFrame/...
     * @param buttonNames automatically create items with names of buttonNames
     * @param defaultSelection is the index to show as selected on start
     * @param cbUserSelection a callback to invoke with index of pressed button
     * @param pos location in the Layout (if the layout supports Point locations)
     * @return a smart pointer copy of the object kept in the Layout
     * @code
        Canvas c(image.size());
        auto selectionBox = RadioButtons::create(c, {
                                                "Long Option1",     // index 0
                                                "Option2\n2 lines", // index 1
                                                "Option3"           // index 2
                                            },
                                            [](Widget *w, int i) {
            w->setVisible(false);
            cout << "Option " << i << " was chosen" << endl;
        });
        while(...)
        {
            c.redrawOn(...);
            imshow(...);
            key = waitKeyEx(...);
        }
     * @endcode
     */
    static std::shared_ptr<RadioButtons> create(Layout &layoutVal,
                                                std::vector<std::string> buttonNames,
                                                int defaultSelection = -1,
                                                Widget::CBUserSelection cbUserSelection = Widget::CBUserSelection(),
                                                const cv::Point &pos = cv::Point(0,0));

    /**
     * @brief setSelection
     * make a certain option selected
     * @param value will be shown as selected
     */
    void setSelection(int value);

    /// get the current selected option
    int getSelection() const;

    virtual const char *getType() const;

    static const char *type;

protected:
    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;

    RadioButtons(Layout &layoutVal, const cv::Point &pos);

    void addRadioButton(const std::string &txt,
                        Widget::CBUserSelection cbUserSelection);

    virtual void recalc();

private:

    cv::Mat getRadioNotSelected();
    cv::Mat getRadioSelected();

    std::shared_ptr<VFrame> frame;
    std::vector<MatWidget*> drawings;
    int selection;
};

}

/** @example example_radiobuttons.cpp
 * This is an example of how to use the RadioButtons Widget.
 */

#endif // RADIOBUTTONS_H
