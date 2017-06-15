#ifndef BUTTON_H
#define BUTTON_H

#include <opencv2/opencv.hpp>
#include <list>
#include "colors.h"
#include "floatingtext.h"

namespace canvascv
{

/**
 * @brief The Button class
 * a button widget to use on the OpenCV Window
 */
class Button : public FloatingText
{
public:
    virtual const char *getType() const;

    /**
     * @brief create a button widget
     * @param layout widgets are placed in layouts Canvas/VFrame/HFrame/...
     * @param pos location in the Layout (Layouts can ignore that)
     * @param buttonText what to write on the button
     * @param statusMsg what to write during hover over the button
     * @param maxWidthVal maximum width of the text rows on the button (0 to disable max width)
     * @return a smart pointer copy of the object kept in the Layout
     */
    static std::shared_ptr<Button> create(Layout &layout,
                                          const cv::Point &pos,
                                          const std::string &buttonText,
                                          const std::string &statusMsg="",
                                          int maxWidthVal = 0);

    /// a convinient version to the above without the 'pos' argument
    static std::shared_ptr<Button> create(Layout &layout,
                                          const std::string &buttonText,
                                          const std::string &statusMsg="",
                                          int maxWidthVal = 0);

    static const char *type;

protected:

    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;
    Button(Layout &layoutVal, const cv::Point &pos);

    Button(Layout &layoutVal, const std::string msgVal = "",
           cv::Point locationVal = cv::Point(0, 0),
           int maxWidthVal = 0,
           cv::Scalar colorVal = Colors::BLACK,
           cv::Scalar bgColorVal = Colors::P1_GRAY,
           double fontScaleVal = 0.5,
           int fontThicknessVal = 1,
           int fontFaceVal = cv::FONT_HERSHEY_COMPLEX_SMALL);

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
};

}
#endif // BUTTON_H
