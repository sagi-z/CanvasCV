#include "quadrilateral.h"


namespace canvasvc
{

void Quadrilateral::draw(cv::Mat &canvas)
{
    for (int i = 0; i<4; ++i)
    {
        int first = i;
        int second = i+1;
        if (second == 4)
        {
            second = 0;
        }
        if (! pts[second])
        {
            break;
        }
        line(canvas,(*pts[first])(), (*pts[second])(), outlineColor, thickness, lineType);
    }
    CompoundShape::draw(canvas);
}

bool Quadrilateral::mousePressed(const cv::Point &pos, bool onCreate)
{
    if (! onCreate && CompoundShape::mousePressed(pos, onCreate))
    {
        return true;
    }

    if (! pts[3])
    {
        if (! onCreate)
        {
            for (int i = 1; i<4; ++i)
            {
                if (! pts[i])
                {
                    cout << "Creating point number " << i << endl;
                    pts[i] = addShape<Handle>(pos);
                    pts[i]->setThickness(3);
                    break;
                }
            }
        }
        return true;
    }

    if (selectedPoint1) {
        selectedPoint1->setVisible(false);
        selectedPoint2->setVisible(false);
    }
    if (isPointOnLine(pos, selectedPoint1, selectedPoint2))
    {
        selectedPoint1->setVisible(true);
        selectedPoint2->setVisible(true);
        return true;
    }

    return false;
}

bool Quadrilateral::mouseMoved(const cv::Point &pos)
{
    if (CompoundShape::mouseMoved(pos))
    {
        return true;
    }

    if (! pts[3])
    {
        return true;
    }

    return false;
}

bool Quadrilateral::mouseReleased(const cv::Point &pos)
{
    if (CompoundShape::mouseReleased(pos))
    {
        return true;
    }

    if (! pts[3])
    {
        return true;
    }

    if (selectedPoint1) {
        selectedPoint1->setVisible(false);
        selectedPoint2->setVisible(false);
    }
    if (isPointOnLine(pos, selectedPoint1, selectedPoint2))
    {
        selectedPoint1->setVisible(true);
        selectedPoint2->setVisible(true);
        return true;
    }

    return false;
}

std::list<Handle *> Quadrilateral::getConnectionTargets()
{
    return {pts[0], pts[1], pts[2], pts[3]};
}

}
