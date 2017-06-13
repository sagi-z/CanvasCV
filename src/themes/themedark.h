#ifndef THEMEDARK_H
#define THEMEDARK_H

#include "theme.h"

namespace canvascv
{

/**
 * @brief The ThemeDark class implements a dark theme of orange over dark gray colors
 */
class ThemeDark : public Theme
{
public:
    virtual void applyStyle(Widget *widget);
    virtual void applyStyle(Shape *shape);

    virtual void allocateBG(cv::Mat &dst, const cv::Size &size, const cv::Scalar &color, int type = CV_8UC3);
    virtual void drawBG(cv::Mat &dst, const cv::Rect &rect, const cv::Mat &bg,
                        double alpha, bool fillBG, const cv::Scalar &bgColor);
    virtual void flat(cv::Mat &bg, const cv::Scalar &color);
    virtual void raised(cv::Mat &bg, const cv::Scalar &color);
    virtual void sunken(cv::Mat &bg, const cv::Scalar &color);
    virtual void selected(cv::Mat &bg, const cv::Scalar &color);
};

}
#endif // THEMEDARK_H
