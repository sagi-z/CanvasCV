#ifndef MSGBOX_H
#define MSGBOX_H

#include <future>
#include "compoundwidget.h"
#include "vframe.h"
#include "horizontallayout.h"

namespace canvascv
{

class Canvas;

/**
 * @brief The MsgBox class
 * 
 * Use a message box with any number of buttons on an OpenCV window
 */
class MsgBox : public CompoundWidget
{
public:

    /**
     * @brief create a message box widget which is closed automatically
     * 
     * The MsgBox is not modal - does not block input to other widgets.
     * For a modal MsgBox inside a Canvas use create() and getUserSelection(true).
     * @param canvas must be a Canvas reference for a MsgBox
     * @param msg what to display in the MsgBox
     * @param buttonNames automatically create buttons with names of buttonNames
     * @param cbUserSelection a callback to invoke with index of pressed button
     * @param pos location in the Canvas (the default is the center of the Canvas)
     * @return a smart pointer copy of the object kept in the Layout
     * @code
        Canvas c(winName, image.size());
        auto msgBox = MsgBox::create(c,
                                     "This is a MsgBox example\n"
                                     "with 2 lines", {
                                         "Ok",           // 0
                                         "Cancel",       // 1
                                         "Somthing else" // 2
                                     });

        while(...)
        {
            if (msgBox && msgBox->getUserSelection() != -1)
            {
                cout << "MsgBox was pressed with key index " << msgBox->getUserSelection() << endl;
                msgBox.reset();
            }
            c.redrawOn(...);
            imshow(...);
            key = c.waitKeyEx(...); // GUI and callbacks happen here
        }
     * @endcode
     * Or you could use the callback version (see the example)
     * @see getUserSelection
     */
    static std::shared_ptr<MsgBox> create(Canvas &canvas,
                                          const std::string &msg,
                                          std::vector<std::string> buttonNames = {"Ok"},
                                          Widget::CBUserSelection cbUserSelection = Widget::CBUserSelection(),
                                          const cv::Point &pos = cv::Point(-1,-1));

    /**
     * @brief createModal
     * 
     * opens a modal (blocking) MsgBox in it's own window and immediatly waits for a user selection.
     * For a modal MsgBox inside a Canvas use create() and getUserSelection(true).
     * @param title is the title of the new window
     * @param msg what to display in the MsgBox
     * @param buttonNames automatically create buttons with names of buttonNames
     * @param cbUserSelection a callback to invoke with index of pressed button
     * @return return result of getUserSelection()
     * @sa getUserSelection(true)
     */
    static int createModal(const std::string &title,
                           const std::string &msg,
                           std::vector<std::string> buttonNames = {"Ok"},
                           Widget::CBUserSelection cbUserSelection = Widget::CBUserSelection());

    /**
     * @brief getUserSelection
     * 
     * get what the user pressed
     * @param blocking if true, block waiting on the MsgBox until a button is pressed
     * @return returns pressed button index or -1 if not pressed
     */
    int getUserSelection(bool blocking = false);

    /**
     * @brief getTextAt
     * 
     * return the text at index index
     * @param index is the index you want the text for
     * @return return the text at index index or empty string if invalid index
     */
    std::string getTextAt(int index) const;

    /// return nunmber of buttons
    size_t size() const;

    virtual const char *getType() const;

    static const char *type;

protected:
    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;

    MsgBox(const cv::Point &pos);

    void setMsg(const std::string &msg);

    virtual void recalcCompound();

private:
    int userSelection;
    VFrame *frame;
    HorizontalLayout *buttons;
};

}

/** @example example_msgbox.cpp
 * This is an example of how to use the MsgBox Widget.
 */

#endif // MSGBOX_H
