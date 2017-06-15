#ifndef MSGBOX_H
#define MSGBOX_H

#include "compoundwidget.h"
#include "vframe.h"
#include "horizontallayout.h"

namespace canvascv
{

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
     * @brief create a message box widget
     * @param layout widgets are placed in layouts Canvas/VFrame/HFrame/...
     * @param msg what to display in the MsgBox
     * @param pos location in the Layout (Layouts can ignore that)
     * @return a smart pointer copy of the object kept in the Layout
     */
    static std::shared_ptr<MsgBox> create(Layout &layout,
                                          const std::string &msg = "",
                                          const cv::Point &pos = cv::Point(0,0));

    /**
     * @brief setMsg
     * set the message to display in the MsgBox
     * @param msg
     */
    void setMsg(const std::string &msg);

    /**
     * @brief addButton
     * @param buttonTex will be displayed on the button
     * @param cb will be called on Button's ENTER/LEAVE/PRESS/RELEASE
     * @see Widget::notifyOnChange()
     */
    void addButton(const std::string &buttonText, CBType cb);

    virtual const char *getType() const;

    static const char *type;

protected:
    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;

    MsgBox(Layout &layoutVal, const cv::Point &pos);

private:
    std::shared_ptr<VFrame> parts;
    std::shared_ptr<HorizontalLayout> buttons;
};

}

/** @example example_msgbox.cpp
 * This is an example of how to use the MsgBox Widget.
 */

#endif // MSGBOX_H
