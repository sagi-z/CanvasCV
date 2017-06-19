#include "polygon.h"

using namespace std;
using namespace cv;

namespace canvascv
{

const char * Polygon::type = "Polygon";

Polygon::Polygon(const Point &pos)
{
    handles.push_back(addShape<Handle>(pos));
    handles.back()->setLocked(true);
    handles.back()->setVisible(false);
    handles.push_back(addShape<Handle>(pos));
}

void Polygon::draw(Mat &canvas)
{
    auto iter = handles.begin();
    auto nextIter = next(iter);
    for (; nextIter != handles.end(); ++iter, ++nextIter)
    {
        line(canvas,(**iter)(), (**nextIter)(), outlineColor, thickness, lineType);
    }
    if (isReady())
    {
        line(canvas,(*handles.back())(), (*handles.front())(), outlineColor, thickness, lineType);
    }
    CompoundShape::draw(canvas);
}

bool Polygon::mousePressed(const Point &pos, bool onCreate)
{
    if (isReady())
    {
        if (CompoundShape::mousePressed(pos, onCreate))
        {
            return true;
        }

        if (isPointInPoly(pos))
        {
            for (Handle *handle : handles)
            {
                handle->setVisible(true);
            }
            return true;
        }
    }
    else
    {
        if (! onCreate)
        {
            handles.back()->setLocked(true);
            handles.back()->setVisible(false);
            handles.push_back(addShape<Handle>(pos));
        }
        return true;
    }

    return false;
}

bool Polygon::mouseMoved(const Point &pos)
{
    if (isReady())
    {
        if (CompoundShape::mouseMoved(pos))
        {
            return true;
        }
    }
    else
    {
        handles.back()->setPos(pos);
        handles.back()->setVisible(true);
        return true;
    }

    return false;
}

bool Polygon::mouseReleased(const Point &pos)
{
    if (isReady())
    {
        if (CompoundShape::mouseReleased(pos))
        {
            return true;
        }

        if (isPointInPoly(pos))
        {
            for (Handle *handle : handles)
            {
                handle->setVisible(true);
            }
            return true;
        }
    }
    else
    {
        return true; // editing points - active
    }

    return false;
}

void Polygon::lostFocus()
{
    if (! isReady())
    {
        setDeleted();
    }
    else
    {
        for (Handle *handle : handles)
        {
            handle->setVisible(false);
        }
    }
}

list<Handle *> Polygon::getConnectionTargets()
{
    return handles;
}

const char *Polygon::getType() const
{
    return type;
}

bool Polygon::keyPressed(int &key)
{
    if (isReady()) return true;
    if (key < 128 || key == 65288)
    {
        switch (key)
        {
        case -1:
            break;
        case 13: // \r
        case 10: // \n
            key = -1; // consume key
            if (handles.size() > 3)
            {
                setReady();

                // The last handle was just used for show
                //  and is not needed anymore
                handles.back()->setVisible(false);
                handles.pop_back();
                delActiveShape();
                setActive(handles.back());

                getPoints(vertices);
                for (Handle *handle : handles)
                {
                    handle->setLocked(false);
                    handle->setVisible(true);
                }
            }
            break;
        case 27: // ESC
            key = -1; // consume key
            return false; // lostFocus() => the canvas will delete us
        case 8: // BS
        case 65288: // BS on pi3 without QT over VNC
            key = -1; // consume key
            if (handles.size() > 3)
            {
                Handle *toDelete = handles.back();
                toDelete->setVisible(false);
                Point pos = (*toDelete)();
                handles.pop_back();
                handles.back()->setLocked(false);
                handles.back()->setPos(pos);
                delActiveShape();
                setActive(handles.back());
            }
            else
            {
                return false; // lostFocus() => the canvas will delete us
            }
            break;
        }
    }
    return true;
}

const string &Polygon::getStatusMsg() const
{
   const static string msgCreate =
           "Click to create a new vertex.\n"
           "ENTER to finish. ESC to cancel. BS to remove last point.";
   const static string msgEdit = "drag a vertex to modify to polygon";
   if (isReady())
   {
       return msgEdit;
   }
   else
   {
       return msgCreate;
   }
}

void Polygon::translate(const Point &offset)
{
    CompoundShape::translate(offset);
    getPoints(vertices);
}

void Polygon::reloadPointers(const list<Shape*> &lst, list<Shape*>::const_iterator &i)
{
    CompoundShape::reloadPointers(lst, i);
    // Here we know we consume all that's left
    handles.clear();
    while (i != lst.end())
    {
        handles.push_back(dynamic_cast<Handle*>(*i++));
    }
    getPoints(vertices);
    setReady();
}

}
