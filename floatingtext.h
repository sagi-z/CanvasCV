#ifndef FLOATINGTEXT_H
#define FLOATINGTEXT_H

#include <opencv2/opencv.hpp>
#include <list>
#include "colors.h"

namespace canvascv
{

class FloatingText
{
public:
    FloatingText(const std::string msgVal = "",
                 cv::Point topLeftVal = cv::Point(),
                 cv::Scalar colorVal = Colors::BLACK,
                 cv::Scalar bgColorVal = Colors::P1_GRAY,
                 double fontScaleVal = 0.5,
                 int fontThicknessVal = 1,
                 double alphaVal = 0.3,
                 int fontFaceVal = cv::FONT_HERSHEY_COMPLEX_SMALL);

    double getAlpha() const;
    void setAlpha(double value);

    cv::Scalar getColor() const;
    void setColor(const cv::Scalar &value);

    cv::Scalar getBgColor() const;
    void setBgColor(const cv::Scalar &value);

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

private:

    struct LineData
    {
        std::string str;
        int width;
    };

    void prepareMsgParts();

    std::string msg;
    cv::Point topLeft;
    cv::Scalar color;
    cv::Scalar bgColor;
    double alpha;
    std::list<LineData> msgParts;
    int fontHeight;
    int fontFace;
    double fontScale;
    int fontThickness;
};

}
#endif // FLOATINGTEXT_H
