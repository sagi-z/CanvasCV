#include "ellipse.h"

namespace canvasvc
{

Ellipse::Ellipse(const Point &pos)
    : Rectangle(pos)
{
}

void Ellipse::draw(Mat &canvas)
{
    cv::ellipse(canvas, getRect(), outlineColor, thickness, lineType);
    CompoundShape::draw(canvas);
}

}
