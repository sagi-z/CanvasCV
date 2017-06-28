#include "shapesconnector.h"
#include "canvas.h"

#include <iterator>

using namespace std;
using namespace cv;

namespace canvascv
{

const char * ShapesConnector::type = "ShapesConnector";

ShapesConnector::ShapesConnector(const Point &pos)
    : Line(pos),
      tailShape(0),
      tailHandle(0),
      headShape(0),
      headHandle(0),
      space(5),
      tmpDragTarget(nullptr)
{
    center = addShape<Handle>(pos);
    center->setLocked(true);
    setOutlineColor(Colors::Orange);
    registerCBs();
}

void ShapesConnector::draw(Mat &canvas)
{
    LineIterator lineIter(canvas, (*pt1)(), (*pt2)());
    for(int i = 0; i < lineIter.count; i++,lineIter++)
    {
        if ( i%space==0 )
        {
            circle(canvas, lineIter.pos(), thickness, fillColor, -1, lineType);
            circle(canvas, lineIter.pos(), thickness, outlineColor, 1, lineType);
        }
    }
    CompoundShape::draw(canvas);
}

bool ShapesConnector::mousePressed(const Point &pos, bool onCreate)
{
    if (isPointOnLine(pos))
    {
        center->setVisible(true);
    }
    return Line::mousePressed(pos, onCreate);
}

bool ShapesConnector::mouseReleased(const Point &pos)
{
    if ((! tailShape && pt1->isAtPos(pos))
            || (! headShape && pt2->isAtPos(pos)))
    {
        for (auto &targetHandle : dragTargetHandles)
        {
            targetHandle->setVisible(false);
        }
        if (tmpDragTarget)
        {
            tmpDragTarget->setOutlineColor(tmpDragTargetColor);
            tmpDragTarget->setThickness(tmpDragTargetThickness);
            tmpDragTarget = nullptr;
        }
        dragTargetHandles.clear();
        for (auto &shape: dragTargetShapes)
        {
            if (shape.get() != this)
            {
                list<Handle*> handles = shape->getConnectionTargets();
                for (Handle *targetHandle : handles)
                {
                    if (targetHandle->isAtPos(pos))
                    {
                        if (pt1->isAtPos(pos))
                        {
                            connectTail(*shape, *targetHandle);
                        }
                        else
                        {
                            connectHead(*shape, *targetHandle);
                        }
                        break;
                    }
                }
            }
        }
        dragTargetShapes.clear();
    }
    return Line::mouseReleased(pos);
}

const string &ShapesConnector::getCreateStatusMsg() const
{
   static const string msg = "Hover over a shape, then click on it's circled handle.";
   return msg;
}

const string &ShapesConnector::getEditStatusMsg() const
{
   static const string notConnectedMsg = "Drag an edge over a shape, then release on it's circled handle.";
   static const string connectedMsg = "Dragging the connecter will drag along it's coonnected shapes.";
   if (tailShape && headShape)
   {
       return connectedMsg;
   }
   else
   {
       return notConnectedMsg;
   }
}

list<Handle *> ShapesConnector::getConnectionTargets()
{
    return  list<Handle *>({center});
}

const char *ShapesConnector::getType() const
{
    return type;
}

int ShapesConnector::getSpacing() const
{
    return space;
}

void ShapesConnector::setSpacing(int value)
{
    space = value;
    if (space < 1)
    {
        space = 1;
    }
}

void ShapesConnector::connectTail(Shape &shape, Handle &handle)
{
    if (connectToEndPoint(shape, handle, *pt1))
    {
        tailShape = shape.getId();
        tailHandle = handle.getId();
    }
    pt1->setPos(handle(), false);
    pt1->delPosChangedCB(tailCBID);
    pt1->setLocked(true);
}

void ShapesConnector::connectHead(Shape &shape, Handle &handle)
{
    if (connectToEndPoint(shape, handle, *pt2))
    {
        headShape = shape.getId();
        headHandle = handle.getId();
    }
    pt2->setPos(handle(), false);
    pt2->delPosChangedCB(headCBID);
    pt2->setLocked(true);
}

void ShapesConnector::disconnectTail()
{
    if (tailShape)
    {
        DisconnectEndPoint(tailHandle, *pt1);
        tailShape = 0;
        tailHandle = 0;
    }
    tailCBID = registerConnectCB(tailShape, pt1);
    pt1->setLocked(false);
}

void ShapesConnector::disconnectHead()
{
    if (headShape)
    {
        DisconnectEndPoint(headHandle, *pt2);
        headShape = 0;
        headHandle = 0;
    }
    pt2->setLocked(false);
    headCBID = registerConnectCB(headShape, pt2);
}

void ShapesConnector::reconnect()
{
   if (tailShape && tailHandle)
   {
       shared_ptr<Shape> pShape = canvas->getShape(tailShape);
       shared_ptr<Shape> pHandle = canvas->getShape(tailHandle);
       if (pShape.get() && pHandle.get())
       {
           Handle *handle = dynamic_cast<Handle*>(pHandle.get());
           if (handle)
           {
               connectTail(*pShape, *handle);
           }
       }
   }

   if (headShape && headHandle)
   {
       shared_ptr<Shape> pShape = canvas->getShape(headShape);
       shared_ptr<Shape> pHandle = canvas->getShape(headHandle);
       if (pShape.get() && pHandle.get())
       {
           Handle *handle = dynamic_cast<Handle*>(pHandle.get());
           if (handle)
           {
               connectHead(*pShape, *handle);
           }
       }
   }
}

Shape *ShapesConnector::getTailShape()
{
    return getShapeFromCanvas(tailShape);
}

Shape *ShapesConnector::getHeadShape()
{
    return getShapeFromCanvas(headShape);
}

void ShapesConnector::disconnectShape(int id)
{
   if (id == tailShape)
   {
       disconnectTail();
   }
   else if (id == headShape)
   {
       disconnectHead();
   }
}

void ShapesConnector::translate(const Point &offset)
{
    Shape *pHead = getHeadShape();
    if (pHead && pHead->getLocked()) return;

    Shape *pTail = getTailShape();
    if (pTail && pTail->getLocked()) return;

    if (pHead && pTail)
    {
        pHead->translate(offset);
        pTail->translate(offset);
    }
    else if (!pHead && !pTail)
    {
        CompoundShape::translate(offset);
    }
    else
    {
        if (!pHead && getActive() == pt2) pt2->translate(offset);
        else if (!pTail && getActive() == pt1) pt1->translate(offset);
    }
}

void ShapesConnector::writeInternals(FileStorage &fs) const
{
    Line::writeInternals(fs);
    fs << "tailShape" << tailShape;
    fs << "tailHandle" << tailHandle;
    fs << "headShape" << headShape;
    fs << "headHandle" << headHandle;
    fs << "space" << space;
}

void ShapesConnector::readInternals(const FileNode &node)
{
    Line::readInternals(node);
    node["tailShape"] >> tailShape;
    node["tailHandle"] >> tailHandle;
    node["headShape"] >> headShape;
    node["headHandle"] >> headHandle;
    node["space"] >> space;
}

Shape *ShapesConnector::getShapeFromCanvas(int id)
{
   if (id == 0)
   {
       return nullptr;
   }
   return canvas->getShape(id).get();
}

bool ShapesConnector::connectToEndPoint(Shape &shape, Handle &handle, Handle &endPoint)
{
    list<Handle*> handles = shape.getConnectionTargets();
    if (find(handles.begin(), handles.end(), &handle) != handles.end())
    {
        handle.connect(endPoint);
        return true;
    }
    return false;
}

void ShapesConnector::DisconnectEndPoint(int handleId, Handle &endPoint)
{
    shared_ptr<Shape> pHandle = canvas->getShape(handleId);
    if (pHandle.get())
    {
        Handle *handle = dynamic_cast<Handle*>(pHandle.get());
        if (handle)
        {
            handle->disconnect(endPoint);
        }
    }
}

void ShapesConnector::registerCBs()
{
    registerCenterUpdateCB(pt1);
    registerCenterUpdateCB(pt2);
    tailCBID = registerConnectCB(tailShape, pt1);
    headCBID = registerConnectCB(headShape, pt2);
}

Handle::CBID ShapesConnector::registerCenterUpdateCB(Handle *handle)
{
    return handle->addPosChangedCB([this](const Point &)
    {
        const Point &p1 = getTail();
        const Point &p2 = getHead();
        center->setPos(p1+(p2-p1)/2);
    });
}

Handle::CBID ShapesConnector::registerConnectCB(const int &shapeId, Handle *handle)
{
    return handle->addPosChangedCB([this, &shapeId](const Point &pos)
    {
        if (shapeId == 0)
        {
            for (auto &targetHandle : dragTargetHandles)
            {
                targetHandle->setVisible(false);
                if (tmpDragTarget)
                {
                    tmpDragTarget->setOutlineColor(tmpDragTargetColor);
                    tmpDragTarget->setThickness(tmpDragTargetThickness);
                    tmpDragTarget = nullptr;
                }
            }
            dragTargetHandles.clear();
            canvas->getShapes(pos, dragTargetShapes);
            for (auto &shape: dragTargetShapes)
            {
                if (shape.get() != this)
                {
                    list<Handle*> handles = shape->getConnectionTargets();
                    move(handles.begin(), handles.end(), back_inserter(dragTargetHandles));
                }
            }
            for (auto &targetHandle : dragTargetHandles)
            {
                targetHandle->setVisible(true);
                if (! tmpDragTarget && targetHandle->isAtPos(pos))
                {
                    tmpDragTarget = targetHandle;
                    tmpDragTargetColor=tmpDragTarget->getOutlineColor();
                    tmpDragTargetThickness=tmpDragTarget->getThickness();
                    tmpDragTarget->setThickness(tmpDragTargetThickness*3);
                    tmpDragTarget->setOutlineColor(Colors::Red);
                }
            }
        }
    });
}

}
