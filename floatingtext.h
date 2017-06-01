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
                 cv::Scalar colorVal = Colors::BLACK,
                 cv::Scalar bgColorVal = Colors::P1_GRAY,
                 double fontScaleVal = 0.5,
                 int fontThicknessVal = 1,
                 double alphaVal = 0.3,
                 int fontFaceVal = cv::FONT_HERSHEY_COMPLEX_SMALL);

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

    static const char *type;

protected:
    virtual void writeInternals(cv::FileStorage &fs) const
    {
    }
    virtual void readInternals(const cv::FileNode &node)
    {
    }

private:

    virtual void mousePressed() {}
    virtual void mouseReleased() {}
    virtual void mouseEnter() {}
    virtual void mouseLeave() {}

    struct LineData
    {
        std::string str;
        int width;
    };

    void prepareMsgParts();

    std::string msg;
    cv::Point leftPos;
    double alpha;
    std::list<LineData> msgParts;
    int fontHeight;
    int fontFace;
    double fontScale;
    int fontThickness;
    FlowDirection flowDirection;
    cv::Rect rect;
};

}
#endif // FLOATINGTEXT_H
