#include "layoutbasewidget.h"

using namespace cv;
using namespace std;

namespace canvascv
{

void LayoutBaseWidget::draw(Mat &dst)
{
    if (drawFrame)
    {
        drawBG(dst, rect, colorRect);
    }
    CompoundWidget::draw(dst);
}

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
    : CompoundWidget(pos),
      drawFrame(false)
{
}

bool LayoutBaseWidget::isDuringUpdate() const
{
    return delayedUpdate == false;
}

void LayoutBaseWidget::recalc()
{
    CompoundWidget::recalc();
   if (drawFrame)
   {
       colorRect = Mat(rect.size(), CV_8UC3, fillColor);
   }
}

void LayoutBaseWidget::setDirtyLayout()
{
    setDirty();
}

bool LayoutBaseWidget::getDrawFrame() const
{
    return drawFrame;
}

void LayoutBaseWidget::setDrawFrame(bool value)
{
    if (drawFrame != value)
    {
        drawFrame = value;
        setDirty();
    }
}

}
