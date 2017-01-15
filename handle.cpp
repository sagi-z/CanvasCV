#include "colors.h"
#include "handle.h"

#include <opencv2/imgproc.hpp>

namespace canvasvc
{

Handle::~Handle()
{
   for (auto &handlePair : connectedHandles)
   {
      handlePair.first->disconnectFrom(*this);
   }
}

void Handle::draw(cv::Mat &canvas)
{
    if (visible)
    {
        circle(canvas, pt, thickness, fillColor, -1, lineType);
        circle(canvas, pt, thickness, outlineColor, 1, lineType);
    }
}

bool Handle::mousePressed(const cv::Point &pos, bool)
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

bool Handle::mouseMoved(const cv::Point &pos)
{
    if (editing)
    {
        pt = pos;
        broadcastPosChanged(pos);
        return true;
    }
    return false;
}

bool Handle::mouseReleased(const cv::Point &pos)
{
    if (editing)
    {
        pt = pos;
        broadcastPosChanged(pos);
        editing = false;
    }
    return false;
}

void Handle::writeInternals(cv::FileStorage &fs) const
{
    Shape::writeInternals(fs);
    fs << "pt" << pt;
}

void Handle::readInternals(const cv::FileNode &node)
{
    Shape::readInternals(node);
    node["pt"] >> pt;
}

// this is a private method.
// validation done on public method of peer Handle.
void Handle::connectedFrom(Handle &other)
{
    CBID id = addPosChangedCB([this, &other](const cv::Point &pos)
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

void Handle::broadcastPosChanged(const cv::Point &pos)
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
           CBID id = addPosChangedCB([this, &other](const cv::Point &pos)
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

std::list<Handle *> Handle::getConnectionTargets()
{
    return std::list<Handle *>();
}

}
