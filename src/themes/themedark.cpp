#include "themedark.h"
#include "widgets/widget.h"
#include "shapes/shape.h"
#include "shapes/linecrossing.h"

using namespace cv;

namespace canvascv
{

const Scalar THM_DARK_GRAY(32,32,32,255);
const Scalar THM_LIGHT_GRAY(160,160,160,255);
const Scalar THM_BLACK(0,0,0,255);
const Scalar THM_ORANGE(0,165,255,255);

void ThemeDark::allocateBG(cv::Mat &dst, const cv::Size &size, const cv::Scalar &color)
{
    if (size.width == 0 || size.height == 0)
    {
        dst = cv::Mat();
    }
    else
    {
        dst.create(size, CV_8UC4);
        dst = color;
    }
}

void ThemeDark::flat(Mat &bg, const Scalar &color)
{
    Scalar myDarkGray = THM_DARK_GRAY;
    myDarkGray[3] = color[3];
    cv::rectangle(bg, {0, 0}, {bg.cols - 1, bg.rows - 1}, myDarkGray, 2, LINE_AA);
}

void ThemeDark::raised(Mat &bg, const Scalar &color)
{
    Scalar myLightGray = THM_LIGHT_GRAY;
    Scalar myBlack = THM_BLACK;
    myLightGray[3] = myBlack[3] = color[3];
    // bright top line first
    cv::line(bg, {0,0}, {bg.cols - 1,0}, myLightGray, 2, LINE_AA);

    // dark bottom line second
    cv::line(bg, {bg.cols - 1,bg.rows - 1}, {0,bg.rows - 1}, myBlack, 2, LINE_AA);

    // bright right line third
    cv::line(bg, {bg.cols - 1,0}, {bg.cols - 1,bg.rows - 1}, myLightGray, 2, LINE_AA);

    // dark left line forth
    cv::line(bg, {0,bg.rows - 1}, {0,0}, myBlack, 2, LINE_AA);
}

void ThemeDark::sunken(Mat &bg, const Scalar &color)
{
    Scalar myLightGray = THM_LIGHT_GRAY;
    Scalar myBlack = THM_BLACK;
    myLightGray[3] = myBlack[3] = color[3];
    // reverse light from raised
    // dark top line first
    cv::line(bg, {0,0}, {bg.cols - 1,0}, myBlack, 2, LINE_AA);

    // light bottom line second
    cv::line(bg, {bg.cols - 1,bg.rows - 1}, {0,bg.rows - 1}, myLightGray, 2, LINE_AA);

    // dark right line third
    cv::line(bg, {bg.cols - 1,0}, {bg.cols - 1,bg.rows - 1}, myBlack, 2, LINE_AA);

    // light left line forth
    cv::line(bg, {0,bg.rows - 1}, {0,0}, myLightGray, 2, LINE_AA);
}

void ThemeDark::selected(Mat &bg, const Scalar &color)
{
    Scalar myOrange = THM_ORANGE;
    myOrange[3] = color[3];
    cv::rectangle(bg, {0, 0}, {bg.cols - 1, bg.rows - 1}, myOrange, 2, LINE_AA);
}

void ThemeDark::applyStyle(Widget *widget)
{
   widget->setOutlineColor(THM_ORANGE);
   widget->setFillColor(THM_DARK_GRAY);
   widget->setThickness(1);
   widget->setSelectColor(THM_ORANGE);
}

void ThemeDark::applyStyle(Shape *shape)
{
   shape->setOutlineColor(THM_DARK_GRAY);
   shape->setFillColor(THM_ORANGE);
   shape->setThickness(2);
   if (shape->getType() == LineCrossing::type)
   {
       ((LineCrossing*)shape)->setArrowMagnitude(30);
       ((LineCrossing*)shape)->getArrow()->setOutlineColor(THM_ORANGE);
   }
   else if (shape->getType() == Handle::type)
   {
       shape->setThickness(4);
   }
   else if (shape->getType() == TextBox::type)
   {
       ((TextBox*)shape)->setFontColor(THM_ORANGE);
   }
}

}
