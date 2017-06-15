#ifndef FLOATINGTEXT_H
#define FLOATINGTEXT_H

#include <opencv2/opencv.hpp>
#include <list>
#include "colors.h"
#include "widget.h"

namespace canvascv
{

/**
 * @brief The FloatingText class
 * Displaying text on the OpenCV Window with/without a hilighting background.
 */
class FloatingText : public Widget
{
public:
    /**
     * @brief create a FloatingText widget
     * @param layout widgets are placed in layouts Canvas/VFrame/HFrame/...
     * @param pos location in the Layout (Layouts can ignore that)
     * @param text what to write (accepts newline)
     * @param flowAnchor affects the widget behavior
     *  - TOP/BOTTOM - text appears below or above pos
     *  - RIGHT/CENTER/LEFT is row alignment for multirows
     * @param layoutAnchor affects the layout in the Layout it belongs to
     *  - TOP/CENTER/BOTTOM is relevat to a HorizontalLayout/HFrame (unless setStretchY() is used)
     *  - RIGHT/CENTER/LEFT is relevat to a VerticalLayout/VFrame (unless setStretchX() is used)
     * @return a smart pointer copy of the object kept in the Layout
     */
    static std::shared_ptr<FloatingText> create(Layout &layout, const cv::Point &pos,
                                                const std::string &text = "",
                                                Anchor flowAnchor = TOP,
                                                Anchor layoutAnchor = TOP);

    /// a convinient version to the above without the 'pos' argument
    static std::shared_ptr<FloatingText> create(Layout &layout,
                                                const std::string &text = "",
                                                Anchor flowAnchor = TOP,
                                                Anchor layoutAnchor = TOP);

    virtual const char *getType() const;

    /// get the current text of the FLoatingText
    std::string getMsg() const;

    /// set the current text of the FLoatingText
    void setMsg(const std::string &value);

    /// get the OpenCV font used
    int getFontFace() const;

    /// set the OpenCV font to use
    void setFontFace(int value);

    /// get the OpenCV scale used for te font
    double getFontScale() const;

    /// set the OpenCV scale used for te font
    void setFontScale(double value);

    /// get the calculated for height
    int getFontHeight() const;

    /// get the maxuimum allowed width of rows in pixels (0 means it's disabled)
    int getMaxWidth() const;

    /// set the maxuimum allowed width of rows in pixels (0 to disable max width)
    void setMaxWidth(int value);

    virtual const cv::Rect &getRect();
    virtual const cv::Rect &getMinimalRect();

    /// for FloatingText this method is public
    virtual void setFillBG(bool value);

    static const char *type;

protected:
    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;

    FloatingText(Layout &layoutVal, const cv::Point &pos);

    FloatingText(Layout &layoutVal, const std::string msgVal = "",
                 cv::Point locationVal = cv::Point(0, 0),
                 int maxWidthVal = 0,
                 cv::Scalar colorVal = Colors::Black,
                 cv::Scalar bgColorVal = Colors::P1_GRAY,
                 double fontScaleVal = 0.5,
                 int fontThicknessVal = 1,
                 int fontFaceVal = cv::FONT_HERSHEY_COMPLEX_SMALL);

    virtual void recalc();

    virtual void drawFG(cv::Mat &dst);

    /* TODO - write/read widgets to file for a designer app
    virtual void writeInternals(cv::FileStorage &fs) const
    {
    }
    virtual void readInternals(const cv::FileNode &node)
    {
    }
    */

private:

    void prepareMsgParts();

    struct StringRow
    {
        std::string str;
        int width;
    };

    std::string msg;
    std::list<StringRow> rows;
    int fontHeight;
    int fontFace;
    double fontScale;
    int maxWidth;
    cv::Rect rect;
    cv::Rect minimalRect;
};

}
#endif // FLOATINGTEXT_H
