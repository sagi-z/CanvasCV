#ifndef CHECKBOXES_H
#define CHECKBOXES_H

#include <future>
#include "compoundwidget.h"
#include "vframe.h"

namespace canvascv
{

class MatWidget;

/**
 * @brief The CheckBoxes class
 * 
 * Use a check box to get a user selection for multiple options
 */
class CheckBoxes : public CompoundWidget
{
public:

    /**
     * @brief create a check box widget
     * 
     * @param layoutVal widgets are placed in layouts Canvas/VFrame/HFrame/...
     * @param checkBoxNames automatically create items with names of checkBoxNames
     * @param cbUserSelection a callback to invoke with index of pressed checkBox
     * @param pos location in the Layout (if the layout supports Point locations)
     * @return a smart pointer copy of the object kept in the Layout
     * @code
        Canvas c(winName, image.size());
        auto selectionBoxes = CheckBoxes::create(c, {
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
            key = c.waitKeyEx(...); // GUI and callbacks happen here
        }
     * @endcode
     */
    static std::shared_ptr<CheckBoxes> create(Layout &layoutVal,
                                              std::vector<std::string> checkBoxNames,
                                              Widget::CBUserSelection cbUserSelection = Widget::CBUserSelection(),
                                              const cv::Point &pos = cv::Point(0,0));

    /**
     * @brief setSelection
     * 
     * make a certain box checked
     * @param index is box index
     * @param checked is true or false for checked or unchecked
     */
    void setChecked(int index, bool checked);

    /**
     * @brief isChecked
     * 
     * is a box at index checked
     * @param index is box index
     * @return true or flase for checked or unchecked
     */
    bool isChecked(int index) const;

    /**
     * @brief getTextAt
     * 
     * return the text at index index
     * @param index is the index you want the text for
     * @return return the text at index index or empty string if invalid index
     */
    std::string getTextAt(int index) const;

    /// return nunmber of check boxes in group
    size_t size() const;

    virtual const char *getType() const;

    static const char *type;

protected:
    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;

    CheckBoxes(const cv::Point &pos);

    void addCheckBox(const std::string &txt,
                     Widget::CBUserSelection cbUserSelection);

    virtual void recalcCompound();

private:

    cv::Mat getCheckBoxNotSelected();
    cv::Mat getCheckBoxSelected();

    std::shared_ptr<VFrame> frame;
    std::vector<MatWidget*> drawings;
    cv::Mat checkBoxNotSelected;
    cv::Mat checkBoxSelected;
    std::vector<bool> selections;
};

}

/** @example example_checkboxes.cpp
 * This is an example of how to use the CheckBoxes Widget.
 */

#endif // CHECKBOXES_H
