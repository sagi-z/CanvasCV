#ifndef CANVAS_H
#define CANVAS_H

#include "shape.h"
#include "widget.h"
#include "shapefactory.h"
#include "widgetfactory.h"
#include "floatingtext.h"
#include "layoutbase.h"

#include <list>
#include <memory>
#include <functional>

namespace canvascv
{

class Canvas : public LayoutBase
{
public:
    typedef std::function<void(Shape*)> CBType;

    Canvas(cv::Size sizeVal = cv::Size());

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

    /// Create shape on the canvas directly from code
    std::shared_ptr<Shape> createShape(std::string type, const cv::Point &pos = cv::Point(0,0));

    template <class T>
    std::shared_ptr<T> createShape(const cv::Point &pos = cv::Point(0,0));

    /// Create widget on the canvas directly from code
    std::shared_ptr<Widget> createWidget(const cv::Point &pos, std::string type);

    template <class T>
    std::shared_ptr<T> createWidget(const cv::Point &pos = cv::Point(0,0));

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
     * @brief set default shape type to draw (could be "")
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
     * @brief delete specific shape
     */
    void deleteShape(const std::shared_ptr<Shape> &shape);

    /**
     * @brief delete specific widget
     */
    void deleteWidget(const std::shared_ptr<Widget> &widget);

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

    void disableScreenText()
    {
        hasScreenText = false;
    }

    void disableStatusMsg()
    {
        hasStatusMsg = false;
    }

    void enableScreenText(cv::Scalar color = Colors::BLACK,
                          cv::Scalar bgColor = Colors::P1_GRAY,
                          double scale = 0.5,
                          int thickness = 1,
                          double alpha = 0.3,
                          int fontFace = FONT_HERSHEY_COMPLEX_SMALL);

    void enableStatusMsg(cv::Scalar color = Colors::P1_ORANGE,
                         cv::Scalar bgColor = Colors::P1_GRAY,
                         double scale = 0.5,
                         int thickness = 1,
                         double alpha = 0.3,
                         int fontFace = FONT_HERSHEY_COMPLEX_SMALL);

    void setStatusMsg(const std::string &msg);

    void setScreenText(const std::string &msg);

    virtual void recalc() {}

    virtual cv::Size getAllowedSize() const;
    void setSize(const cv::Size &value);

    virtual void addWidget(const std::shared_ptr<Widget> &widget);
    virtual bool rmvWidget(const std::shared_ptr<Widget> &widget);

    virtual bool rmvWidget(Widget *widget);
private:

    virtual void setDirtyLayout();

    void broadcastCreate(Shape *shape);
    void broadcastModify(Shape *shape);
    void broadcastDelete(Shape *shape);

    void processNewShape();

    cv::Size size;
    bool hasScreenText;
    bool hasStatusMsg;
    FloatingText screenText;
    FloatingText statusMsg;
    std::string shapeType;
    std::list<std::shared_ptr<Shape>> shapes;
    std::list<std::shared_ptr<Widget>> widgets;
    std::shared_ptr<Shape> active;
    std::shared_ptr<Widget> activeWidget;
    std::list<CBType> createNotifs;
    std::list<CBType> modifyNotifs;
    std::list<CBType> deleteNotifs;

    friend void write(cv::FileStorage& fs, const std::string&, const Canvas& x);
    friend void read(const cv::FileNode& node, Canvas& x, const Canvas&);
};

template <class T>
std::shared_ptr<T> Canvas::createShape(const cv::Point &pos)
{
    std::shared_ptr<T> shape(ShapeFactoryT<T>::newShape(pos));
    shapes.push_back(shape);
    processNewShape();
    shape->lostFocus();
    return shape;
}

template <class T>
std::shared_ptr<T> Canvas::createWidget(const cv::Point &pos)
{
    std::shared_ptr<T> widget(WidgetFactoryT<T>::newWidget(pos));
    widget->setLayout(*this);
    widgets.push_back(widget);
    return widget;
}

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
