#ifndef SHAPESCONNECTOR_H
#define SHAPESCONNECTOR_H

#include "line.h"

namespace canvascv
{

class ShapesConnector : public Line
{
public:
    ShapesConnector(const cv::Point &pos);

    virtual void draw(Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false);
    virtual bool mouseReleased(const cv::Point &pos);
    virtual std::list<Handle *> getConnectionTargets();
    virtual const char *getType() const {
        return type;
    }

    int getSpacing() const;
    void setSpacing(int value);

    void connectTail(Shape &shape, Handle &handle);
    void connectHead(Shape &shape, Handle &handle);
    void disconnectTail();
    void disconnectHead();

    // called after read of Canvas from file
    void reconnect();

    int getTailShapeId()
    {
        return tailShape;
    }

    int getHeadShapeId()
    {
        return headShape;
    }

    Shape *getTailShape();
    Shape *getHeadShape();

    void disconnectShape(int id);

    static const char * type;

protected:
    Handle *center;

    virtual void writeInternals(cv::FileStorage &fs) const;
    virtual void readInternals(const cv::FileNode &node);


    virtual void reloadPointers(std::list<Shape*>::const_iterator &i)
    {
        Line::reloadPointers(i);
        center = dynamic_cast<Handle*>(*i++);
        registerCBs();
    }

private:

    Shape *getShapeFromCanvas(int id);
    bool connectToEndPoint(Shape &shape, Handle &handle, Handle &endPoint);
    void DisconnectEndPoint(int handleId, Handle &endPoint);
    void registerCBs();
    Handle::CBID registerCenterUpdateCB(Handle *handle);
    Handle::CBID registerConnectCB(const int &shapeId, Handle *handle);

    int tailShape;
    int tailHandle;
    int headShape;
    int headHandle;
    int space;
    std::list<std::shared_ptr<Shape>> dragTargetShapes;
    std::list<Handle*> dragTargetHandles;
    Handle::CBID tailCBID;
    Handle::CBID headCBID;
    Handle *tmpDragTarget;
    cv::Scalar tmpDragTargetColor;
    int tmpDragTargetThickness;
};

}

#endif // SHAPESCONNECTOR_H
