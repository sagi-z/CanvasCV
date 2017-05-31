#include "rectangle.h"

namespace canvascv
{

const char * Rectangle::type = "Rectangle";

void Rectangle::draw(Mat &canvas)
{
    cv::line(canvas, (*pt1)(), (*pt2)(), outlineColor, thickness, lineType);
    cv::line(canvas, (*pt2)(), (*pt3)(), outlineColor, thickness, lineType);
    cv::line(canvas, (*pt3)(), (*pt4)(), outlineColor, thickness, lineType);
    cv::line(canvas, (*pt4)(), (*pt1)(), outlineColor, thickness, lineType);
    CompoundShape::draw(canvas);
}

bool Rectangle::mousePressed(const cv::Point &pos, bool onCreate)
{
    if (CompoundShape::mousePressed(pos, onCreate))
    {
        return true;
    }

    if (isPointInRectangle(pos))
    {
        for (auto &pt : {pt1, pt2, pt3, pt4, rotate1, rotate2, rotate3, rotate4, center})
        {
            pt->setVisible(true);
        }
        return true;
    }

    return false;
}

bool Rectangle::mouseMoved(const cv::Point &pos)
{
    if (CompoundShape::mouseMoved(pos))
    {
        return true;
    }

    if (isPointInRectangle(pos))
    {
        return true;
    }

    return false;
}

bool Rectangle::mouseReleased(const cv::Point &pos)
{
    if (CompoundShape::mouseReleased(pos))
    {
        return true;
    }

    if (isPointInRectangle(pos))
    {
        // mouse unclicked in rect
        for (auto &pt : {pt1, pt2, pt3, pt4, rotate1, rotate2, rotate3, rotate4, center})
        {
            pt->setVisible(true);
        }
        return true;
    }

    return false;
}

void Rectangle::writeInternals(cv::FileStorage &fs) const
{
    CompoundShape::writeInternals(fs);
    fs << "width" << width;
    fs << "height" << height;
    fs << "angle" << angle;
}

void Rectangle::readInternals(const cv::FileNode &node)
{
    CompoundShape::readInternals(node);
    node["width"] >> width;
    node["height"] >> height;
    node["angle"] >> angle;
}

std::list<Handle *> Rectangle::getConnectionTargets()
{
    return {rotate1, rotate2, rotate3, rotate4};
}

}
