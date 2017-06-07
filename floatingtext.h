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

    FloatingText(const cv::Point &pos);

    FloatingText(const std::string msgVal = "",
                 cv::Point locationVal = cv::Point(0, 0),
                 int maxWidthVal = 0,
                 cv::Scalar colorVal = Colors::BLACK,
                 cv::Scalar bgColorVal = Colors::P1_GRAY,
                 double fontScaleVal = 0.5,
                 int fontThicknessVal = 1,
                 double alphaVal = 0.5,
                 int fontFaceVal = cv::FONT_HERSHEY_COMPLEX_SMALL);

    static std::shared_ptr<FloatingText> create(Layout &layout, const cv::Point &pos,
                                                const std::string &text,
                                                Anchor flowAnchor = TOP_LEFT,
                                                Anchor layoutAnchor = TOP_LEFT);

    static std::shared_ptr<FloatingText> create(Layout &layout,
                                                const std::string &text,
                                                Anchor flowAnchor = TOP_LEFT,
                                                Anchor layoutAnchor = TOP_LEFT);

    virtual const char *getType() const;

    double getAlpha() const;
    void setAlpha(double value);

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

    // TODO
    virtual void writeInternals(cv::FileStorage &fs) const
    {
    }
    // TODO
    virtual void readInternals(const cv::FileNode &node)
    {
    }

    double alpha;
private:

    virtual void mousePressed() {}
    virtual void mouseReleased() {}
    virtual void mouseEnter() {}
    virtual void mouseLeave() {}

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
    cv::Mat rectColor;
};

}
#endif // FLOATINGTEXT_H
