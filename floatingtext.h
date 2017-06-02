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
    enum FlowDirection
    {
        TOP_DOWN,
        BOTTOM_UP
    };

    FloatingText(cv::Point pos);

    FloatingText(const std::string msgVal = "",
                 cv::Point topLeftVal = cv::Point(),
                 int maxWidthVal = 0,
                 cv::Scalar colorVal = Colors::BLACK,
                 cv::Scalar bgColorVal = Colors::P1_GRAY,
                 double fontScaleVal = 0.5,
                 int fontThicknessVal = 1,
                 double alphaVal = 0.5,
                 int fontFaceVal = cv::FONT_HERSHEY_COMPLEX_SMALL);

    static std::shared_ptr<FloatingText> newFloatingText(Canvas &c,
                                                         cv::Point pos,
                                                         const std::string &text,
                                                         int maxWidthVal = 0,
                                                         FlowDirection flow = TOP_DOWN);

    virtual bool isAtPos(const cv::Point &pos);

    virtual const char *getType() const;

    double getAlpha() const;
    void setAlpha(double value);

    std::string getMsg() const;
    void setMsg(const std::string &value);

    int getFontFace() const;
    void setFontFace(int value);

    double getFontScale() const;
    void setFontScale(double value);

    int getFontThickness() const;
    void setFontThickness(int value);

    void draw(cv::Mat &dst);

    cv::Point getTopLeft() const;
    void setTopLeft(const cv::Point &value);

    int getFontHeight() const;

    cv::Point getLeftPos() const;
    void setLeftPos(const cv::Point &value);

    FlowDirection getFlowDirection() const;
    void setFlowDirection(const FlowDirection &value);

    int getMaxWidth() const;
    void setMaxWidth(int value);

    static const char *type;

protected:
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

    virtual void canvasResized(const cv::Size &size);
    virtual void mousePressed() {}
    virtual void mouseReleased() {}
    virtual void mouseEnter() {}
    virtual void mouseLeave() {}

    void prepareMsgParts();

    std::string msg;
    cv::Point leftPos;
    std::list<std::string> rows;
    int fontHeight;
    int fontFace;
    double fontScale;
    int fontThickness;
    FlowDirection flowDirection;
    int maxWidth;
    int yStart;
    cv::Rect rect;
    cv::Mat rectColor;
};

}
#endif // FLOATINGTEXT_H
