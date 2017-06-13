#include "themedark.h"
#include "colors.h"
#include "widgets/widget.h"
#include "shapes/shape.h"
#include "shapes/linecrossing.h"

using namespace cv;

namespace canvascv
{

void ThemeDark::allocateBG(cv::Mat &dst, const cv::Size &size, const cv::Scalar &color, int type)
{
    (void) color; // unused in this theme
    dst.create(size, type);
//    dst = Colors::M3_GRAY;
    dst = color;
}

void ThemeDark::drawBG(cv::Mat &dst, const cv::Rect &rect, const cv::Mat &bg,
                       double alpha, bool fillBG)
{
    Mat roi = dst(rect);
    if (fillBG)
    {
        if (alpha > 0. && alpha < 1.)
        {
            cv::addWeighted(bg, alpha, roi, 1.0 - alpha , 0.0, roi);
        }
        else
        {
            bg.copyTo(roi);
        }
    }
}

void ThemeDark::flat(Mat &bg, const Scalar &color)
{
    (void) color; // unused in this theme
    cv::rectangle(bg, {0, 0}, {bg.cols - 1, bg.rows - 1}, Colors::M3_GRAY, 2, LINE_AA);
}

void ThemeDark::raised(Mat &bg, const Scalar &color)
{
    (void) color; // unused in this theme
    // bright top line first
    cv::line(bg, {0,0}, {bg.cols - 1,0}, Colors::P1_GRAY, 2, LINE_AA);

    // dark bottom line second
    cv::line(bg, {bg.cols - 1,bg.rows - 1}, {0,bg.rows - 1}, Colors::BLACK, 2, LINE_AA);

    // bright right line third
    cv::line(bg, {bg.cols - 1,0}, {bg.cols - 1,bg.rows - 1}, Colors::P1_GRAY, 2, LINE_AA);

    // dark left line forth
    cv::line(bg, {0,bg.rows - 1}, {0,0}, Colors::BLACK, 2, LINE_AA);
}

void ThemeDark::sunken(Mat &bg, const Scalar &color)
{
    (void) color; // unused in this theme
    // reverse light from raised
    // dark top line first
    cv::line(bg, {0,0}, {bg.cols - 1,0}, Colors::BLACK, 2, LINE_AA);

    // light bottom line second
    cv::line(bg, {bg.cols - 1,bg.rows - 1}, {0,bg.rows - 1}, Colors::P1_GRAY, 2, LINE_AA);

    // dark right line third
    cv::line(bg, {bg.cols - 1,0}, {bg.cols - 1,bg.rows - 1}, Colors::BLACK, 2, LINE_AA);

    // light left line forth
    cv::line(bg, {0,bg.rows - 1}, {0,0}, Colors::P1_GRAY, 2, LINE_AA);
}

void ThemeDark::selected(Mat &bg, const Scalar &color)
{
    (void) color; // unused in this theme
    cv::rectangle(bg, {0, 0}, {bg.cols - 1, bg.rows - 1}, Colors::P1_ORANGE, 2, LINE_AA);
}

void ThemeDark::applyStyle(Widget *widget)
{
   widget->setOutlineColor(Colors::P1_ORANGE);
   widget->setFillColor(Colors::M3_GRAY);
   widget->setThickness(1);
   widget->setSelectColor(Colors::P1_ORANGE);
}

void ThemeDark::applyStyle(Shape *shape)
{
   shape->setOutlineColor(Colors::M3_GRAY);
   shape->setFillColor(Colors::P1_ORANGE);
   shape->setThickness(1);
   if (shape->getType() == LineCrossing::type)
   {
       ((LineCrossing*)shape)->setArrowMagnitude(30);
       ((LineCrossing*)shape)->getArrow()->setOutlineColor(Colors::P1_ORANGE);
   }
   else if (shape->getType() == Handle::type)
   {
       shape->setThickness(3);
   }
}

}
