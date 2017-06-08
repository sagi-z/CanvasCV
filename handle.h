#ifndef HANDLE_H
#define HANDLE_H

#include "shape.h"
#include "shapefactory.h"

#include <opencv2/opencv.hpp>
#include <functional>
#include <list>

namespace canvascv
{

class Handle : public Shape
{
public:
    typedef std::function<void(const cv::Point &)> PosChangedCB;
    typedef std::list<PosChangedCB>::iterator CBID;

    Handle(const cv::Point &pos);

    // force compiler to generate default cctor inspite of the ctor
    //  we defined above
    Handle(const Handle &) = default;

    ~Handle();

    virtual bool isAtPos(const cv::Point &pos)
    {
        return isPoint(pos);
    }

    virtual std::list<Handle *> getConnectionTargets();

    virtual const char *getType() const;

    const cv::Point &operator()() const {
        return pt;
    }

    void setPos(const cv::Point &pos, bool notify = true);

    int getRadius() const;

    CBID addPosChangedCB(PosChangedCB cb);

    void delPosChangedCB(const CBID &id);

    void connect(Handle &other);
    void disconnect(Handle &other);

    static const char * type;

protected:
    virtual void writeInternals(cv::FileStorage &fs) const;
    virtual void readInternals(const cv::FileNode &node);

    virtual void draw(cv::Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool = false);
    virtual bool mouseMoved(const cv::Point &pos);
    virtual bool mouseReleased(const cv::Point &pos);
    virtual void lostFocus();

private:

    void connectedFrom(Handle &other);
    void disconnectFrom(Handle &other);

    bool isPoint(const cv::Point &pos, int threshold=3)
    {
        threshold += thickness/2;
        return abs(pos.x - pt.x) <= threshold &&
                abs(pos.y - pt.y) <= threshold;
    }

    void broadcastPosChanged(const cv::Point& pos);

    std::list<PosChangedCB> posChangedCBs;
    std::map<Handle*, CBID> connectedHandles;
    cv::Point pt;
    bool allowSetPos;
};

}

#endif // HANDLE_H


