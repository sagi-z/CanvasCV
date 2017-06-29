#include "themedarkorange.h"
#include "canvascv/widgets/widget.h"
#include "canvascv/shapes/shape.h"
#include "canvascv/shapes/linecrossing.h"

using namespace cv;

namespace canvascv
{

ThemeDarkOrange::ThemeDarkOrange()
    : TwoColoredTheme(Colors::Orange,
                      Scalar(32,32,32)) // special dark gray
{
}

}
