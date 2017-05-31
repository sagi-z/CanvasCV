#include "line.h"
#include <opencv2/imgproc.hpp>

namespace canvascv
{

const char * Line::type = "Line";

void Line::draw(cv::Mat &canvas)
{
    line(canvas,(*pt1)(), (*pt2)(), outlineColor, thickness, lineType);
    CompoundShape::draw(canvas);
}

bool Line::mousePressed(const cv::Point &pos, bool onCreate)
{
    if (CompoundShape::mousePressed(pos, onCreate))
    {
        return true;
    }

    if (isPointOnLine(pos))
    {
        if (! pt1->getLocked())
        {
            pt1->setVisible(true);
        }
        if (! pt2->getLocked())
        {
            pt2->setVisible(true);
        }
        if (! onCreate && ! locked && ! dragDisabled)
        {
            dragPos = pos;
        }
        return true;
    }

    return false;
}

bool Line::mouseMoved(const cv::Point &pos)
{
    if (dragPos.x || dragPos.y)
    {
        cv::Point diff = dragPos - pos;
        pt1->setPos((*pt1)() - diff);
        pt2->setPos((*pt2)() - diff);
        dragPos = pos;
        return true;
    }

    if (CompoundShape::mouseMoved(pos))
    {
        return true;
    }

    if (isPointOnLine(pos))
    {
        return true;
    }

    return false;
}

bool Line::mouseReleased(const cv::Point &pos)
{
    if (dragPos.x || dragPos.y)
    {
        dragPos.x = 0;
        dragPos.y = 0;
        return true;
    }

    if (CompoundShape::mouseReleased(pos))
    {
        return true;
    }

    if (isPointOnLine(pos))
    {
        // mouse unclicked on line
        return true;
    }

    return false;
}

std::list<Handle *> Line::getConnectionTargets()
{
    return {pt1, pt2};
}

}
