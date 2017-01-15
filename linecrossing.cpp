#include "linecrossing.h"
#include <opencv2/imgproc.hpp>

namespace canvasvc {

void LineCrossing::draw(cv::Mat &canvas)
{
    if (line->length()<10)
    {
        arrow->setVisible(false);
    } else {
        arrow->setVisible(true);
        arrow->getPT1().setVisible(false);
        arrow->getPT2().setVisible(false);
    }
    CompoundShape::draw(canvas);
}

bool LineCrossing::mousePressed(const cv::Point &pos, bool onCreate)
{
    if (arrow->getVisible())
    {
        if (arrow->isPointOnLine(pos))
        {
            direction *= -1;
            recalcArrow();
            return true;
        }
    }

    if (CompoundShape::mousePressed(pos, onCreate))
    {
        return true;
    }
    return false;
}

void LineCrossing::writeInternals(cv::FileStorage &fs) const
{
    CompoundShape::writeInternals(fs);
    fs << "direction" << direction;
}

void LineCrossing::readInternals(const cv::FileNode &node)
{
    CompoundShape::readInternals(node);
    node["direction"] >> direction;
    registerCBs();
}


std::list<Handle *> LineCrossing::getConnectionTargets()
{
    return textBox->getConnectionTargets();
}

}
