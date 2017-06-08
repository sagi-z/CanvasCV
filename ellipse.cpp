#include "ellipse.h"

using namespace std;
using namespace cv;

namespace canvascv
{

const char * Ellipse::type = "Ellipse";

Ellipse::Ellipse(const Point &pos)
    : Rectangle(pos)
{
}

const char *Ellipse::getType() const
{
    return type;
}

void Ellipse::draw(Mat &canvas)
{
    ellipse(canvas, getRect(), outlineColor, thickness, lineType);
    CompoundShape::draw(canvas);
}

}
