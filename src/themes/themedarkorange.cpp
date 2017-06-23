#include "themedarkorange.h"
#include "widgets/widget.h"
#include "shapes/shape.h"
#include "shapes/linecrossing.h"

using namespace cv;

namespace canvascv
{

ThemeDarkOrange::ThemeDarkOrange()
    : TwoColoredTheme(Colors::Orange,
                      Scalar(32,32,32)) // special dark gray
{
}

}
