#ifndef CANVAS_H
#define CANVAS_H

#include "shapes/shape.h"
#include "widgets/widget.h"
#include "shapes/shapefactory.h"
#include "widgets/widgetfactory.h"
#include "widgets/floatingtext.h"
#include "widgets/layoutbase.h"

#include <list>
#include <memory>
#include <functional>

namespace canvascv
{

/**
 * @brief The Canvas class is the entry point into CanvasCV
 * This is conceptually a canvas layer on top of your frame.
 *  - It can be turned on and off.
 *  - It can show your messages on screen with a semi transparent background.
 *  - You can handle mouse and keyboard events:
 *      -# To create/edit/delete shapes on screen for user selections and landmark configuration.
 *      -# To handle widgets you put on the screen for user input.
 */
class Canvas : public LayoutBase
{
public:
    typedef std::function<void(Shape*)> CBType;

    /**
     * @brief Canvas
     * This class is associated with an OpenCV window of a certain size.
     * @param sizeVal is the size of the OpenCV window
     * @see setSize()
     */
    Canvas(cv::Size sizeVal = cv::Size());

    /// release all shapes and widgets. Shape callbacks are invoked doe delete.
    ~Canvas();

    /**
     * @brief redrawOn draws the shapes on dst
     * @param src can be also dst, in which case it is drawn on.
     * @param dst if different than src, then src is cloned to it and drawn on.
     */
    void redrawOn(const cv::Mat &src, cv::Mat &dst);

    /// You should delegate OpenCV mouse callback events to this method - returns true if did something at pos
    bool onMousePress(const cv::Point &pos);

    /// You should delegate OpenCV mouse callback events to this method
    void onMouseRelease(const cv::Point &pos);

    /// You should delegate OpenCV mouse callback events to this method
    void onMouseMove(const cv::Point &pos);

    /// Create shape by name on the canvas directly from code (instead of by the user using the mouse)
    std::shared_ptr<Shape> createShape(std::string type, const cv::Point &pos = cv::Point(0,0));

    /// Create shape by type on the canvas directly from code (instead of by the user using the mouse)
    template <class T>
    std::shared_ptr<T> createShape(const cv::Point &pos = cv::Point(0,0));

    /**
     * @brief consumeKey takes a key value and tries to use it in a shape or widget
     * @param key is the key value received from the user.
     * It will change to -1 if it was consumed by a shape or widget.
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
    void notifyOnShapeCreate(CBType cb);

    /**
     * @brief used to register for notifications on shape modification (actually when it is deselected)
     * @param cb to invoke on shape modification
     */
    void notifyOnShapeModify(CBType cb);

    /**
     * @brief used to register for notifications on shape deletion
     * @param cb to invoke on shape deletion
     */
    void notifyOnShapeDelete(CBType cb);

    /**
     * @brief clear all shapes from Canvas
     */
    void clearShapes();

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

    /// disable the top left text area for manual user messages
    void disableScreenText()
    {
        hasScreenText = false;
    }

    /// disable the bottom left text area for auto status messages
    void disableStatusMsg()
    {
        hasStatusMsg = false;
    }

    /**
     * @brief enableScreenText enables the top left text area for manual user messages
     * During enable of this feature you can overide some default values
     * @param color is font color
     * @param bgColor is rect bg color
     * @param scale is font scale
     * @param thickness is font thickness
     * @param alpha is the alpha value of the rect bg [0,255] => [transparent,opaque]
     * @param fontFace is the OpenCV fonr to use
     */
    void enableScreenText(cv::Scalar color = Colors::Black,
                          cv::Scalar bgColor = Colors::LightGray,
                          double scale = 0.5,
                          int thickness = 1,
                          uchar alpha = 80,
                          int fontFace = FONT_HERSHEY_COMPLEX_SMALL);

    /**
     * @brief enableStatusMsg enables the bottom left text area for auto status messages
     * During enable of this feature you can overide some default values
     * @param color is font color
     * @param bgColor is rect bg color
     * @param scale is font scale
     * @param thickness is font thickness
     * @param alpha is the alpha value of the rect bg [0,255] => [transparent,opaque]
     * @param fontFace is the OpenCV fonr to use
     */
    void enableStatusMsg(cv::Scalar color = Colors::Orange,
                         cv::Scalar bgColor = Colors::LightGray,
                         double scale = 0.5,
                         int thickness = 1,
                         uchar alpha = 80,
                         int fontFace = FONT_HERSHEY_COMPLEX_SMALL);

    /// manually send a status message (it might be replaced automatically when using widgets and shapes with the mouse)
    void setStatusMsg(const std::string &msg);

    /// manually set the screen text message. It remains until disabled or changed.
    void setScreenText(const std::string &msg);

    /// set the Canvas size to match the cv::Mat we'll use in redrawOn;
    void setSize(const cv::Size &value);

    /// get the Canvas size
    cv::Size getSize();

    virtual void addWidget(const std::shared_ptr<Widget> &widget);
    virtual bool rmvWidget(const std::shared_ptr<Widget> &widget);

    /// is redrawOn() on/off?
    bool getOn() const;

    /// redrawOn will do nothing if value is 'false'
    void setOn(bool value);

    /// write all the shapes currently in the Canvas to a file
    void writeShapesToFile(const std::string &filepath) const;

    /// load all the from a file into the canvas (removing all current shapes in the process)
    void readShapesFromFile(const std::string &filepath);

protected:
    virtual void recalc() {}

    virtual bool rmvWidget(Widget *widget);


    virtual const cv::Rect getBoundaries() const;
    virtual bool replaceTmpSharedPtr(const std::shared_ptr<Widget> &widget);

private:

    virtual void setDirtyLayout();

    void broadcastCreate(Shape *shape);
    void broadcastModify(Shape *shape);
    void broadcastDelete(Shape *shape);

    void processNewShape();

    bool on;
    cv::Rect boundaries;
    bool hasScreenText;
    bool hasStatusMsg;
    std::shared_ptr<FloatingText> screenText;
    std::shared_ptr<FloatingText> statusMsg;
    std::string shapeType;
    cv::Point dragPos;
    std::list<std::shared_ptr<Shape>> shapes;
    std::list<std::shared_ptr<Widget>> widgets;
    std::shared_ptr<Shape> activeShape;
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
    ((Shape*)shape.get())->setReady();
    ((Shape*)shape.get())->lostFocus();
    activeShape.reset();
    return shape;
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

void writeShapes(cv::FileStorage& fs, const std::string&, const Canvas& x);
void readShapes(const cv::FileNode& node, Canvas& x, const Canvas&);

}

/** @example example_shapes_widgets.cpp
 * This is an example of using shapes and widgets together.
 */


#endif // CANVAS_H
