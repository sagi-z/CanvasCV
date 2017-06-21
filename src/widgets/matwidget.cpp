#include "matwidget.h"
#include "widgetfactory.h"
#include "layout.h"

using namespace cv;
using namespace std;

namespace canvascv
{

const char *MatWidget::type = "MatWidget";

MatWidget::MatWidget(Layout &layoutVal, const Point &pos)
    : Widget(layoutVal, pos)
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
    dst = mat;
}

const Mat &MatWidget::getMat() const
{
    return mat;
}

void MatWidget::setMat(const cv::Mat &value)
{
    if (value.ptr(0) != mat.ptr(0))
    {
        mat = value;
        setDirty();
    }
}

const Rect &MatWidget::getRect()
{
    return rect;
}

const Rect &MatWidget::getMinimalRect()
{
    return rect;
}

void MatWidget::recalc()
{
    rect.x = location.x;
    rect.y = location.y;
    rect.width = mat.cols;
    rect.height = mat.rows;
    callDrawFG(false);
}

}
