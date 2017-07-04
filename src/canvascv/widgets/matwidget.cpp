#include "matwidget.h"
#include "widgetfactory.h"
#include "layout.h"

using namespace cv;
using namespace std;

namespace canvascv
{

const char *MatWidget::type = "MatWidget";

MatWidget::MatWidget(const Point &pos)
    : Widget(pos)
{
}

shared_ptr<MatWidget> MatWidget::create(Layout &layout,
                                        Mat mat,
                                        const cv::Point &pos)
{
    shared_ptr<MatWidget> widget(WidgetFactoryT<MatWidget>::newWidget(layout, pos));
    widget->setMat(mat);
    return widget;
}

const char *MatWidget::getType() const
{
   return type;
}

void MatWidget::drawFG(Mat &dst)
{
    dst = displayedMat;
}

const Mat &MatWidget::getMat() const
{
    return mat;
}

void MatWidget::setMat(const cv::Mat &value)
{
    if (value.ptr(0) != mat.ptr(0))
    {
        mat = displayedMat = value;
        setDirty();
    }
}

const Rect &MatWidget::getRect()
{
    return rect;
}

const Rect &MatWidget::getMinimalRect()
{
    return minimalRect;
}

void MatWidget::recalc()
{
    minimalRect.x = location.x;
    minimalRect.y = location.y;
    minimalRect.width = mat.cols;
    minimalRect.height = mat.rows;
    double fx = 1.;
    double fy = 1.;
    if (forcedWidth && displayedMat.cols != forcedWidth)
    {
        fx = forcedWidth / mat.cols;
    }
    if (forcedHeight && displayedMat.rows != forcedHeight)
    {
        fy = forcedHeight / mat.rows;
    }
    if (fx != 1. || fy != 1.)
    {
        cv::resize(mat, displayedMat, Size(), fx, fy);
    }
    rect.x = location.x;
    rect.y = location.y;
    rect.width = displayedMat.cols;
    rect.height = displayedMat.rows;
    callDrawFG(false);
}

}
