#ifndef THEMEDARK_H
#define THEMEDARK_H

#include "theme.h"

namespace canvascv
{

class ThemeDark : public Theme
{
public:
    virtual void allocateBG(cv::Mat &dst, const cv::Size &size, const cv::Scalar &color, int type = CV_8UC3);
    virtual void applyStateStyle(cv::Mat &bg, int state);
    virtual void drawBG(cv::Mat &dst, const cv::Rect &rect, const cv::Mat &bg,
                        double alpha, bool fillBG, cv::Scalar bgOutline);
    virtual void applyStyle(Widget *widget);
    virtual void applyStyle(Shape *shape);

private:
    void drawOutline(cv::Mat &dst, int state);
};

}
#endif // THEMEDARK_H
