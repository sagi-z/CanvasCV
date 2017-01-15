#include "arrow.h"
#include "shapefactory.h"
#include <opencv2/imgproc.hpp>

namespace canvasvc {

void Arrow::draw(cv::Mat &canvas)
{
    arrowedLine(canvas,(*pt1)(), (*pt2)(), outlineColor, thickness, lineType);
    CompoundShape::draw(canvas);
}

}
