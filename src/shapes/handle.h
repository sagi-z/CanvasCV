#ifndef HANDLE_H
#define HANDLE_H

#include "shape.h"
#include "shapefactory.h"

#include <opencv2/opencv.hpp>
#include <functional>
#include <list>

namespace canvascv
{

/**
 * @brief The Handle class
 * The vertices by which the shape is defined and edited
 */
class Handle : public Shape
{
public:
    typedef std::function<void(const cv::Point &)> PosChangedCB;
    typedef std::list<PosChangedCB>::iterator CBID;

    // force compiler to generate default cctor inspite of the ctor
    //  we defined above
    Handle(const Handle &) = default;

    virtual ~Handle();

    virtual bool isAtPos(const cv::Point &pos)
    {
        return isPoint(pos);
    }

    virtual std::list<Handle *> getConnectionTargets();

    virtual const char *getType() const;

    /// return the Point location of this Handle
    const cv::Point &operator()() const {
        return pt;
    }

    /**
     * @brief setPos
     * set the location of this handle
     * @param pos is the new location to change to
     * @param notify determines if other cbs which want to know when we change should be notified (mostly 'true')
     */
    void setPos(const cv::Point &pos, bool notify = true);

    virtual void translate(const cv::Point &offset);

    /// return the radius of this circled handle
    int getRadius() const;

    /// register to be notified when this handle changes
    CBID addPosChangedCB(PosChangedCB cb);

    /// unregister to be notified when this handle changes
    void delPosChangedCB(const CBID &id);

    /// connect to a handle to change when we change
    void connect(Handle &other);

    /// disconnect from a handle that needed to change with us
    void disconnect(Handle &other);

    static const char * type;

protected:
    friend class ShapeFactory;
    template <class T> friend class ShapeFactoryT;

    Handle(const cv::Point &pos);

    virtual void writeInternals(cv::FileStorage &fs) const;
    virtual void readInternals(const cv::FileNode &node);

    virtual void draw(cv::Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool = false);
    virtual bool mouseMoved(const cv::Point &pos);
    virtual bool mouseReleased(const cv::Point &pos);
    virtual void lostFocus();

    virtual const string &getCreateStatusMsg() const;
    virtual const string &getEditStatusMsg() const;

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


