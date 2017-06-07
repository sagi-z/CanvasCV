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
      drawFrame(false),
      padding(2)
{
    fillBG = false;
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
       if (padding)
       {
           rect.x -= padding;
           rect.y -= padding;
           rect.width += padding*2;
           rect.height += padding*2;
       }
       colorRect = Mat(rect.size(), CV_8UC3, fillColor);
   }
}

void LayoutBaseWidget::setDirtyLayout()
{
    setDirty();
}

int LayoutBaseWidget::getPadding() const
{
    return padding;
}

void LayoutBaseWidget::setPadding(int value)
{
    if (padding != value)
    {
        padding = value;
        setDirty();
    }
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
