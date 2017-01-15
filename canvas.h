#ifndef CANVAS_H
#define CANVAS_H

#include "shape.h"
#include "shapefactory.h"

#include <list>
#include <memory>
#include <functional>

namespace canvasvc
{

class Canvas
{
public:
    typedef std::function<void(Shape*)> CBType;

    Canvas();

    ~Canvas();

    /**
     * @brief redrawOn draws the shapes on dst
     * @param src can be also dst, in which case it is drawn on.
     * @param dst if different than src, then src is cloned to it and drawn on.
     */
    void redrawOn(const cv::Mat &src, cv::Mat &dst);

    void onMousePress(const cv::Point &pos);

    void onMouseRelease(const cv::Point &pos);

    void onMouseMove(const cv::Point &pos);

    /**
     * @brief consumeKey takes a key value and tries to use it
     * @param key is the key value received from the user. It will change to -1 if it was consumed by a shape.
     */
    void consumeKey(int &key);

    /**
     * @brief get default shape type to draw
     * @return returns the default current shape to create on mouse press
     */
    std::string getShapeType() const
    {
        return shapeType;
    }

    /**
     * @brief set default shape type to draw
     * @param value will be the default current shape to create on mouse press
     */
    void setShapeType(std::string value)
    {
        shapeType = value;
    }

    /**
     * @brief delete shape currenty selected
     */
    void deleteActive();

    /**
     * @brief used to register for notifications on shape creation
     * @param cb to invoke on shape creation
     */
    void notifyOnCreate(CBType cb);

    /**
     * @brief used to register for notifications on shape modification (actually when it is deselected)
     * @param cb to invoke on shape modification
     */
    void notifyOnModify(CBType cb);

    /**
     * @brief used to register for notifications on shape deletion
     * @param cb to invoke on shape deletion
     */
    void notifyOnDelete(CBType cb);

    /**
     * @brief clear all shapes from Canvas
     */
    void clear();

    /**
     * @brief getShapes of a specific type
     * @param result will contain alll shapes of the T on return
     * @note these are internal shapes used in the canvas, so changing them affects what
     *  is drawn on the Canvas.
     */
    template <class T>
    void getShapes(std::list<std::shared_ptr<T>> &result);

    /**
     * @brief getShape
     * @param id
     * @return shape with requested id
     */
    std::shared_ptr<Shape> getShape(int id);

    /**
     * @brief getShapes
     * @param pos is position to search for shapes
     * @param result is all the shapes at pos
     */
    void getShapes(const cv::Point &pos, std::list<std::shared_ptr<Shape>> &result);

private:

    void broadcastCreate(Shape *shape);
    void broadcastModify(Shape *shape);
    void broadcastDelete(Shape *shape);

    std::string shapeType;
    std::list<std::shared_ptr<Shape>> shapes;
    std::shared_ptr<Shape> active;
    std::list<CBType> createNotifs;
    std::list<CBType> modifyNotifs;
    std::list<CBType> deleteNotifs;

    friend void write(cv::FileStorage& fs, const std::string&, const Canvas& x);
    friend void read(const cv::FileNode& node, Canvas& x, const Canvas&);
};

template <class T>
void Canvas::getShapes(std::list<std::shared_ptr<T>> &result)
{
    for (auto &shape : shapes)
    {
        std::shared_ptr<T> derived = std::dynamic_pointer_cast<T>(shape);
        if (derived.get())
        {
            result.push_back(derived);
        }
    }
}

void write(cv::FileStorage& fs, const std::string&, const Canvas& x);
void read(const cv::FileNode& node, Canvas& x, const Canvas&);

}

#endif // CANVAS_H
