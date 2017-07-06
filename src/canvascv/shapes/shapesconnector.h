#ifndef SHAPESCONNECTOR_H
#define SHAPESCONNECTOR_H

#include "line.h"

namespace canvascv
{

/**
 * @brief The ShapesConnector class
 * 
 * Allows visually connecting 2 shapes from the code or by mouse
 */
class ShapesConnector : public Line
{
public:

    /// get the spacing of the dotted connecting line
    int getSpacing() const;

    /// set the spacing of the dotted connecting line
    void setSpacing(int value);

    /// Connect 'shape' with it's Handle 'handle' to our own tail Handle
    void connectTail(Shape &shape, Handle &handle);

    /// Connect 'shape' with it's Handle 'handle' to our own head Handle
    void connectHead(Shape &shape, Handle &handle);

    /// disconnect our own tail Handle from any previously connected shape
    void disconnectTail();

    /// disconnect our own head Handle from any previously connected shape
    void disconnectHead();

    /// called after read of Canvas from file, assuming internals are filled, but not connected
    void reconnect();

    /// get the id of the shape connected to our tail Handle
    int getTailShapeId()
    {
        return tailShape;
    }

    /// get the id of the shape connected to our head Handle
    int getHeadShapeId()
    {
        return headShape;
    }

    /// get the shape connected to our tail Handle
    Shape *getTailShape();

    /// get the shape connected to our head Handle
    Shape *getHeadShape();

    /// disconnect a previouslt connected shape with an 'id'
    void disconnectShape(int id);

    virtual void translate(const cv::Point &offset);

    virtual std::list<Handle *> getConnectionTargets();

    virtual const char *getType() const;

    static const char * type;

protected:
    friend class ShapeFactory;
    template <class T> friend class ShapeFactoryT;

    ShapesConnector(const cv::Point &pos);

    virtual void draw(Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false);
    virtual bool mouseReleased(const cv::Point &pos);

    virtual const string &getCreateStatusMsg() const;
    virtual const string &getEditStatusMsg() const;

    Handle *center;

    virtual void writeInternals(cv::FileStorage &fs) const;
    virtual void readInternals(const cv::FileNode &node);


    virtual void reloadPointers(const std::list<Shape*> &lst, std::list<Shape*>::const_iterator &i)
    {
        Line::reloadPointers(lst, i);
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

/** @example example_shapes_widgets.cpp
 * This is an example of how to create shapes by code and mouse.
 */

#endif // SHAPESCONNECTOR_H
