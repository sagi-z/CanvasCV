#include "arrow.h"
#include "shapefactory.h"
#include <opencv2/imgproc.hpp>

namespace canvascv {

const char * Arrow::type = "Arrow";

void Arrow::draw(cv::Mat &canvas)
{
    arrowedLine(canvas,(*pt1)(), (*pt2)(), outlineColor, thickness, lineType);
    CompoundShape::draw(canvas);
}

}
