#include "consts.h"
#include "colors.h"

namespace canvascv
{
    const int Consts::DEFAULT_ALPHA = 128;
    const int Consts::DEFAULT_THICKNESS = 1;
    const cv::Scalar Consts::DEFAULT_FG_COLOR = Colors::Green;
    const cv::Scalar Consts::DEFAULT_BG_COLOR = Colors::Black;
    const cv::Scalar Consts::DEFAULT_SELECT_COLOR = Colors::Orange;
    const int Consts::DEFAULT_FONT = cv::FONT_HERSHEY_COMPLEX_SMALL;
    const double Consts::DEFAULT_FONT_SCALE = 0.55;
    const cv::Scalar Consts::DEFAULT_FONT_COLOR = Colors::Green;
    const int Consts::DEFAULT_FONT_THICKNESS = 1;
    const int Consts::DEFAULT_LAYOUT_SPACING = 5;
    const int Consts::DEFAULT_LAYOUT_PADDING = 2;
}
