#ifndef FLOATINGTEXT_H
#define FLOATINGTEXT_H

#include <opencv2/opencv.hpp>
#include <list>
#include "colors.h"
#include "widget.h"

namespace canvascv
{

class FloatingText : public Widget
{
public:

    FloatingText(Layout &layoutVal, const cv::Point &pos);

    FloatingText(Layout &layoutVal, const std::string msgVal = "",
                 cv::Point locationVal = cv::Point(0, 0),
                 int maxWidthVal = 0,
                 cv::Scalar colorVal = Colors::BLACK,
                 cv::Scalar bgColorVal = Colors::P1_GRAY,
                 double fontScaleVal = 0.5,
                 int fontThicknessVal = 1,
                 int fontFaceVal = cv::FONT_HERSHEY_COMPLEX_SMALL);

    static std::shared_ptr<FloatingText> create(Layout &layout, const cv::Point &pos,
                                                const std::string &text = "",
                                                Anchor flowAnchor = TOP,
                                                Anchor layoutAnchor = TOP);

    static std::shared_ptr<FloatingText> create(Layout &layout,
                                                const std::string &text = "",
                                                Anchor flowAnchor = TOP,
                                                Anchor layoutAnchor = TOP);

    virtual const char *getType() const;

    std::string getMsg() const;
    void setMsg(const std::string &value);

    int getFontFace() const;
    void setFontFace(int value);

    double getFontScale() const;
    void setFontScale(double value);

    int getFontHeight() const;

    int getMaxWidth() const;
    void setMaxWidth(int value);

    virtual const cv::Rect &getRect();
    virtual const cv::Rect &getMinimalRect();

    virtual void translate(const cv::Point &translation);

    static const char *type;

protected:
    virtual void recalc();

    virtual void draw(cv::Mat &dst);

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

    std::string msg;
    std::list<std::string> rows;
    int fontHeight;
    int fontFace;
    double fontScale;
    int maxWidth;
    int yStart;
    cv::Rect rect;
    cv::Rect minimalRect;
};

}
#endif // FLOATINGTEXT_H
