#include "arrow.h"
#include "shapefactory.h"
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

namespace canvascv
{

const char * Arrow::type = "Arrow";

Arrow::Arrow(const Point &pos)
    : Line(pos)
{
}

const char *Arrow::getType() const
{
    return type;
}

void Arrow::draw(Mat &canvas)
{
    arrowedLine(canvas,(*pt1)(), (*pt2)(), outlineColor, thickness, lineType);
    CompoundShape::draw(canvas);
}

}
