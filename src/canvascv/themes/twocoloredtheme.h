#ifndef TWOCOLOREDTHEME_H
#define TWOCOLOREDTHEME_H

#include "theme.h"

namespace canvascv
{

/**
 * @brief The ThemeDark class implements a 2 colored theme base class
 */
class TwoColoredTheme : public Theme
{
public:
    virtual void applyStyle(Widget *widget);
    virtual void applyStyle(Shape *shape);

    virtual void allocateBG(cv::Mat &dst, const cv::Size &size, const cv::Scalar &color);
    virtual void flat(cv::Mat &bg, const cv::Scalar &color);
    virtual void raised(cv::Mat &bg, const cv::Scalar &color);
    virtual void sunken(cv::Mat &bg, const cv::Scalar &color);
    virtual void selected(cv::Mat &bg, const cv::Scalar &color);

protected:
    TwoColoredTheme(const cv::Scalar &fgColorVal, const cv::Scalar &bgColorVal);
    virtual ~TwoColoredTheme();

    const cv::Scalar fgColor;
    const cv::Scalar bgColor;
};

}

/** @example example_add_theme.cpp
 * This is an example of how to add a theme.
 */

#endif // TWOCOLOREDTHEME_H
