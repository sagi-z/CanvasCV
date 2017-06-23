#include "themedarkorange.h"
#include "widgets/widget.h"
#include "shapes/shape.h"
#include "shapes/linecrossing.h"

using namespace cv;

namespace canvascv
{

ThemeDarkOrange::ThemeDarkOrange()
    : TwoColoredTheme(Scalar(0,165,255,255), // dark gray
                      Scalar(32,32,32,255))  // orange
{
}

}
