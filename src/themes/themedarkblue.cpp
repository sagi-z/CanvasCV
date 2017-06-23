#include "themedarkblue.h"
#include "colors.h"

using namespace cv;

namespace canvascv
{

ThemeDarkBlue::ThemeDarkBlue()
    : TwoColoredTheme(Colors::RoyalBlue,
                      Scalar(32,32,32)) // special dark gray
{
}

}
