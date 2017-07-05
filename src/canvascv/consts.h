#ifndef CONSTS_H
#define CONSTS_H

#include <opencv2/opencv.hpp>

namespace canvascv
{

/**
 * @brief The Consts class
 * 
 * The place for global constants constants
 */
class Consts
{
public:
    static const int DEFAULT_ALPHA;
    static const int DEFAULT_THICKNESS;
    static const cv::Scalar DEFAULT_FG_COLOR;
    static const cv::Scalar DEFAULT_BG_COLOR;
    static const cv::Scalar DEFAULT_SELECT_COLOR;
    static const int DEFAULT_FONT;
    static const double DEFAULT_FONT_SCALE;
    static const cv::Scalar DEFAULT_FONT_COLOR;
    static const int DEFAULT_FONT_THICKNESS;
    static const int DEFAULT_LAYOUT_SPACING;
    static const int DEFAULT_LAYOUT_PADDING;
};

}

#endif // CONSTS_H
