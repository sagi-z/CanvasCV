#ifndef TEXT_H
#define TEXT_H

#include <opencv2/opencv.hpp>
#include <list>
#include "colors.h"
#include "widget.h"

namespace canvascv
{

/**
 * @brief The Text class
 * Displaying text on the OpenCV Window with/without a hilighting background.
 */
class Text : public Widget
{
public:
    /**
     * @brief create a Text widget
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
    static std::shared_ptr<Text> create(Layout &layout, const cv::Point &pos,
                                        const std::string &text = "",
                                        Anchor flowAnchor = TOP,
                                        Anchor layoutAnchor = TOP);

    /// a convinient version to the above without the 'pos' argument
    static std::shared_ptr<Text> create(Layout &layout,
                                        const std::string &text = "",
                                        Anchor flowAnchor = TOP,
                                        Anchor layoutAnchor = TOP);

    virtual const char *getType() const;

    /// get the current text of the Text
    std::string getText() const;

    /// set the current text of the Text
    void setText(const std::string &value);

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

    static const char *type;

    virtual const cv::Rect &getRect();
    virtual const cv::Rect &getMinimalRect();

    Text(const cv::Point &pos);

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

    int getPadding() const;
    void setPadding(int value);

private:

    void prepareMsgParts();

    struct StringRow
    {
        std::string str;
        int width;
    };

    int padding;
    int doublePadding;
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
#endif // TEXT_H
