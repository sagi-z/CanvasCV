#ifndef MSGBOX_H
#define MSGBOX_H

#include <future>
#include "compoundwidget.h"
#include "vframe.h"
#include "horizontallayout.h"

namespace canvascv
{

class Canvas;
class VFrame;
class HorizontalLayout;

/**
 * @brief The MsgBox class
 * Use a message box with any number of buttons on an OpenCV window
 */
class MsgBox : public CompoundWidget
{
public:

    /**
     * @brief create a message box widget which is closed automatically
     * @param layout must be a Canvas reference for a MsgBox
     * @param msg what to display in the MsgBox
     * @param buttonNames automatically create buttons with names of buttonNames
     * @param pos location in the Canvas (the default is the center of the Canvas)
     * @return a smart pointer copy of the object kept in the Layout
     * @code
        Canvas c(image.size());
        auto msgBox = MsgBox::create(c,
                                     "This is a MsgBox example\n"
                                     "with 2 lines", {
                                         "Ok",           // 0
                                         "Cancel",       // 1
                                         "Somthing else" // 2
                                     });

        while(...)
        {
            if (msgBox->getUserSelection() != -1)
            {
                cout << "MsgBox was pressed with key index " << msgBox->getUserSelection() << endl;
            }
            c.redrawOn(...);
            imshow(...);
            key = waitKeyEx(...);
        }
     * @endcode
     * @see getSelectedButton
     */
    static std::shared_ptr<MsgBox> create(Canvas &canvas,
                                          const std::string &msg,
                                          std::vector<std::string> buttonNames = {"Ok"},
                                          const cv::Point &pos = cv::Point(-1,-1));

    /// returns pressed button index or -1 if not pressed
    int getUserSelection();

    virtual const char *getType() const;

    static const char *type;

protected:
    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;

    MsgBox(Layout &layoutVal, const cv::Point &pos);

    void setMsg(const std::string &msg);

private:
    int userSelection;
    std::shared_ptr<VFrame> frame;
    std::shared_ptr<HorizontalLayout> buttons;
};

}

/** @example example_msgbox.cpp
 * This is an example of how to use the MsgBox Widget.
 */

#endif // MSGBOX_H
