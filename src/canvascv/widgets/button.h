#ifndef BUTTON_H
#define BUTTON_H

#include <opencv2/opencv.hpp>
#include <list>
#include "canvascv/colors.h"
#include "text.h"

namespace canvascv
{

/**
 * @brief The Button class
 * 
 * a button widget to use on the OpenCV Window
 */
class Button : public Text
{
public:
    typedef std::function<void(Widget*)> CBWidget;

    /**
     * @brief create a button widget
     * 
     * @param layout widgets are placed in layouts Canvas/VFrame/HFrame/...
     * @param pos location in the Layout (Layouts can ignore that)
     * @param buttonText what to write on the button
     * @param statusMsg what to write during hover over the button
     * @param cbVal a callback to invoke when the button is pressed
     * @param maxWidthVal maximum width of the text rows on the button (0 to disable max width)
     * @return a smart pointer copy of the object kept in the Layout
     */
    static std::shared_ptr<Button> create(Layout &layout,
                                          const cv::Point &pos,
                                          const std::string &buttonText,
                                          const std::string &statusMsg="",
                                          CBWidget cbVal = CBWidget(),
                                          int maxWidthVal = 0);

    /// a convinient version to the above without the 'pos' argument
    static std::shared_ptr<Button> create(Layout &layout,
                                          const std::string &buttonText,
                                          const std::string &statusMsg="",
                                          CBWidget cbVal = CBWidget(),
                                          int maxWidthVal = 0);

    /// The button will always have a FLAT relief
    void setFlatButton();

    /// Call a user CB when pressed
    void onPress(CBWidget value);

    virtual const char *getType() const;

    static const char *type;

protected:

    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;
    Button(const cv::Point &pos);

    /* TODO - write/read widgets to file for a designer app
    virtual void writeInternals(cv::FileStorage &fs) const
    {
    }
    // TODO
    virtual void readInternals(const cv::FileNode &node)
    {
    }
    */


private:
    virtual void mousePressed();
    virtual void mouseReleased();
    virtual void mouseEnter();
    virtual void mouseLeave();

    CBWidget cb;
};

}
#endif // BUTTON_H
