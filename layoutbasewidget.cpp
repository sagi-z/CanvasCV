#include "layoutbasewidget.h"

using namespace cv;
using namespace std;

namespace canvascv
{

const Rect LayoutBaseWidget::getBoundaries() const
{
    Rect boundaries = rect;
    if (layout)
    {
        boundaries = layout->getBoundaries();
    }
    if (forcedWidth) boundaries.width = forcedWidth;
    if (forcedHeight) boundaries.height = forcedHeight;
    return boundaries;
}

LayoutBaseWidget::LayoutBaseWidget(const Point &pos)
    : CompoundWidget(pos)
{
}

bool LayoutBaseWidget::isDuringUpdate() const
{
   return delayedUpdate == false;
}

void LayoutBaseWidget::setDirtyLayout()
{
    setDirty();
}

}
