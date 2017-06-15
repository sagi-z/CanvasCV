#include "colors.h"
#include "handle.h"

#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

namespace canvascv
{

const char * Handle::type = "Handle";

Handle::Handle(const Point &pos)
    : pt(pos),
      allowSetPos(true)
{
    thickness = 3;
    outlineColor = Colors::Red;
    fillColor = Colors::Red;
}

Handle::~Handle()
{
    for (auto &handlePair : connectedHandles)
    {
        handlePair.first->disconnectFrom(*this);
    }
}

void Handle::draw(Mat &canvas)
{
    if (visible)
    {
        circle(canvas, pt, thickness, fillColor, -1, lineType);
        circle(canvas, pt, thickness, outlineColor, 1, lineType);
    }
}

bool Handle::mousePressed(const Point &pos, bool)
{
    if (visible && ! locked)
    {
        if (isPoint(pos))
        {
            editing = true;
            return true;
        }
    }
    return false;
}

bool Handle::mouseMoved(const Point &pos)
{
    if (editing)
    {
        pt = pos;
        broadcastPosChanged(pos);
        return true;
    }
    return false;
}

bool Handle::mouseReleased(const Point &pos)
{
    if (editing)
    {
        pt = pos;
        broadcastPosChanged(pos);
        editing = false;
    }
    return false;
}

void Handle::writeInternals(FileStorage &fs) const
{
    Shape::writeInternals(fs);
    fs << "pt" << pt;
}

void Handle::readInternals(const FileNode &node)
{
    Shape::readInternals(node);
    node["pt"] >> pt;
}

// this is a private method.
// validation done on public method of peer Handle.
void Handle::connectedFrom(Handle &other)
{
    CBID id = addPosChangedCB([this, &other](const Point &pos)
    {
        allowSetPos = false;
        other.setPos(pos);
        allowSetPos = true;
    });
    connectedHandles[&other] = id;
}

// this is a private method.
// validation done on public method of peer Handle.
void Handle::disconnectFrom(Handle &other)
{
    const auto &iter = connectedHandles.find(&other);
    delPosChangedCB(iter->second);
    connectedHandles.erase(iter);
}

void Handle::broadcastPosChanged(const Point &pos)
{
    for (auto &cb : posChangedCBs)
    {
        cb(pos);
    }
}

void Handle::lostFocus()
{
    visible = false;
}

void Handle::connect(Handle &other)
{
   if (this != &other)
   {
       if (connectedHandles.find(&other) == connectedHandles.end())
       {
           CBID id = addPosChangedCB([this, &other](const Point &pos)
           {
               allowSetPos = false;
               other.setPos(pos);
               allowSetPos = true;
           });
           connectedHandles[&other] = id;
           other.connectedFrom(*this);
       }
   }
}

void Handle::disconnect(Handle &other)
{
   if (this != &other)
   {
       const auto &iter = connectedHandles.find(&other);
       if (iter != connectedHandles.end())
       {
           delPosChangedCB(iter->second);
           iter->first->disconnectFrom(*this);
           connectedHandles.erase(iter);
       }
   }
}

list<Handle *> Handle::getConnectionTargets()
{
    return list<Handle *>();
}

const char *Handle::getType() const {
    return type;
}

void Handle::setPos(const Point &pos, bool notify)
{
    if (allowSetPos)
    {
        pt = pos;
        if (notify)
        {
            broadcastPosChanged(pos);
        }
    }
}

int Handle::getRadius() const
{
    return thickness;
}

Handle::CBID Handle::addPosChangedCB(Handle::PosChangedCB cb)
{
    posChangedCBs.push_back(cb);
    return --posChangedCBs.end();
}

void Handle::delPosChangedCB(const Handle::CBID &id)
{
    posChangedCBs.erase(id);
}

}
