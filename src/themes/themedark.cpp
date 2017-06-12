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
    dst = Colors::M3_GRAY;
    drawOutline(dst, Widget::LEAVE);
}

void ThemeDark::applyStateStyle(Mat &bg, int state)
{
    drawOutline(bg, state);
}

void ThemeDark::drawBG(cv::Mat &dst, const cv::Rect &rect, const cv::Mat &bg,
                       double alpha, bool fillBG, Scalar bgOutline)
{
    (void) bgOutline; // unused in this theme
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

void ThemeDark::applyStyle(Widget *widget)
{
   widget->setOutlineColor(Colors::P1_ORANGE);
   widget->setFillColor(Colors::M3_GRAY);
   widget->setThickness(1);
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

void ThemeDark::drawOutline(Mat &dst, int state)
{
    if (state == Widget::LEAVE || state == Widget::RELEASE)
    {   // Button is raised top-right gets the light, which comes from the right:
        // bright top line first
        cv::line(dst, {0,0}, {dst.cols - 1,0}, Colors::P1_GRAY, 2, LINE_AA);

        // dark bottom line second
        cv::line(dst, {dst.cols - 1,dst.rows - 1}, {0,dst.rows - 1}, Colors::BLACK, 2, LINE_AA);

        // bright right line third
        cv::line(dst, {dst.cols - 1,0}, {dst.cols - 1,dst.rows - 1}, Colors::P1_GRAY, 2, LINE_AA);

        // dark left line forth
        cv::line(dst, {0,dst.rows - 1}, {0,0}, Colors::BLACK, 2, LINE_AA);
    }
    else if (state == Widget::ENTER)
    {   // got focus - just draw a rect
        cv::rectangle(dst, {0, 0}, {dst.cols - 1, dst.rows - 1}, Colors::P1_ORANGE, 2, LINE_AA);

        /*
        // dark top line first
        cv::line(dst, {0,0}, {dst.cols - 1,0}, Colors::BLACK, 2, LINE_AA);

        // dark bottom line second
        cv::line(dst, {dst.cols - 1,dst.rows - 1}, {0,dst.rows - 1}, Colors::BLACK, 2, LINE_AA);

        // dark left line third
        cv::line(dst, {0,dst.rows - 1}, {0,0}, Colors::BLACK, 2, LINE_AA);

        // bright right line forth
        cv::line(dst, {dst.cols - 1,0}, {dst.cols - 1,dst.rows - 1}, Colors::P1_GRAY, 2, LINE_AA);
        */
    }
    else // state == Widget::PRESS
    {   // Button is sunken - reverse light from raised
        // dark top line first
        cv::line(dst, {0,0}, {dst.cols - 1,0}, Colors::BLACK, 2, LINE_AA);

        // light bottom line second
        cv::line(dst, {dst.cols - 1,dst.rows - 1}, {0,dst.rows - 1}, Colors::P1_GRAY, 2, LINE_AA);

        // dark right line third
        cv::line(dst, {dst.cols - 1,0}, {dst.cols - 1,dst.rows - 1}, Colors::BLACK, 2, LINE_AA);

        // light left line forth
        cv::line(dst, {0,dst.rows - 1}, {0,0}, Colors::P1_GRAY, 2, LINE_AA);

    }
}

}
