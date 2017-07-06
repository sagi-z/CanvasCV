#ifndef CANVAS_H
#define CANVAS_H

#include "canvascv/colors.h"
#include "canvascv/consts.h"

#include "shapes/shape.h"
#include "widgets/widget.h"
#include "shapes/shapefactory.h"
#include "widgets/widgetfactory.h"
#include "widgets/text.h"
#include "widgets/layoutbase.h"

#include <list>
#include <memory>
#include <functional>
#include <sstream>

namespace canvascv
{

/**
 * @brief The Canvas class is the entry point into CanvasCV
 * 
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
    /// notification on shapes create/modify/delete from this Canvas instance
    typedef std::function<void(Shape*)> CBCanvasShape;

    /**
     * @brief Canvas
     * 
     * This class is associated with an OpenCV window of a certain size.
     * @param winNameVal is the name of the OpenCV window (it doesn't have to exist yet)
     * @param sizeVal is the size of the OpenCV window
     * @see setSize()
     */
    Canvas(const std::string &winNameVal, cv::Size sizeVal = cv::Size());

    /// release all shapes and widgets. Shape callbacks are invoked doe delete.
    ~Canvas();

    /**
     * @brief redrawOn draws the shapes on dst
     * 
     * Draws src with shapes and widgets onto dst. src is upgraded to 3 channels if it has 1 channel.
     * @param src can be also dst, in which case it is drawn on. src is BGR/BGRA/GRAY.
     * @param dst if different than src, then src is cloned to it and drawn on.
     * @sa setImage
     */
    void redrawOn(const cv::Mat &src, cv::Mat &dst);


    /**
     * @brief redrawOn
     *
     * A utility method that uses latest Mat used as 'src' in the redrawOn above.
     * This methos is also used internally in waitKeyEx(0)
     * @param dst will be used as output.
     * @sa waitKeyEx
     */
    void redrawOn(cv::Mat &dst);

    /**
     * @brief setImage
     *
     * sets the internal 'latestFrameSrc' used for GUI updates while waitKeyEx(0) is used, or with redrawOn(cv::Mat&)
     * @param img will be used to setSize and update internal latestFrameSrc
     */
    void setImage(const cv::Mat &img);

    /// You should delegate OpenCV mouse callback events to this method - returns true if did something at pos
    bool onMousePress(const cv::Point &pos);

    /// You should delegate OpenCV mouse callback events to this method
    void onMouseRelease(const cv::Point &pos);

    /// You should delegate OpenCV mouse callback events to this method
    void onMouseMove(const cv::Point &pos);

    /**
     * @brief createShape
     * 
     * Create shape by name on the canvas directly from code (instead of by the user using the mouse)
     * @param type name of the Shape
     * @param pos shape position
     * @return This method will return a shared_ptr<T> instance, which you don't have to keep
     * since another one is kept by the Canvas in which the shape is placed. Never use
     * delete on a Shape pointer.
     */
    std::shared_ptr<Shape> createShape(std::string type, const cv::Point &pos = cv::Point(0,0));

    /**
     * @brief createShape
     * 
     * Create shape by type on the canvas directly from code (instead of by the user using the mouse)
     * @param pos shape position
     * @return This method will return a shared_ptr<T> instance, which you don't have to keep
     * since another one is kept by the Canvas in which the shape is placed. Never use
     * delete on a Shape pointer.
     */
    template <class T>
    std::shared_ptr<T> createShape(const cv::Point &pos = cv::Point(0,0));

    /**
     * @brief get default shape type to draw
     * 
     * @return returns the default current shape to create on mouse press
     */
    std::string getShapeType() const
    {
        return shapeType;
    }

    /**
     * @brief set default shape type to draw (could be "")
     * 
     * @param value will be the default current shape to create on mouse press
     */
    void setShapeType(std::string value)
    {
        shapeType = value;
    }

    /**
     * @brief delete shape currenty selected
     * 
     */
    void deleteActive();

    /**
     * @brief delete specific shape
     * 
     */
    void deleteShape(const std::shared_ptr<Shape> &shape);

    /**
     * @brief delete specific widget
     * 
     */
    void deleteWidget(const std::shared_ptr<Widget> &widget);

    /**
     * @brief used to register for notifications on shape creation
     *
     * Multiple registrations are allowed.
     * @param cb to invoke on shape creation
     */
    void notifyOnShapeCreate(CBCanvasShape cb);

    /**
     * @brief used to register for notifications on shape modification (actually when it is deselected)
     * 
     * Multiple registrations are allowed.
     * @param cb to invoke on shape modification
     */
    void notifyOnShapeModify(CBCanvasShape cb);

    /**
     * @brief used to register for notifications on shape deletion
     * 
     * Multiple registrations are allowed.
     * @param cb to invoke on shape deletion
     */
    void notifyOnShapeDelete(CBCanvasShape cb);

    /**
     * @brief clear all shapes from Canvas
     *
     */
    void clearShapes();

    /**
     * @brief clear all widgets from Canvas
     * 
     */
    void clearWidgets();

    /**
     * @brief getShapes of a specific type
     * 
     * @param result will contain alll shapes of the T on return
     * @note these are internal shapes used in the canvas, so changing them affects what
     *  is drawn on the Canvas.
     */
    template <class T>
    void getShapes(std::list<std::shared_ptr<T>> &result);

    /**
     * @brief getShape
     * 
     * @param id
     * @return shape with requested id
     */
    std::shared_ptr<Shape> getShape(int id);

    /**
     * @brief getShapes
     * 
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
     * 
     * During enable of this feature you can overide some default values. It is safe to
     * call this method again just to change display settings.
     * @param color is font color
     * @param bgColor is rect bg color
     * @param scale is font scale
     * @param thickness is font thickness
     * @param alpha is the alpha value of the rect bg [0,255] => [transparent,opaque]
     * @param fontFace is the OpenCV fonr to use
     */
    void enableScreenText(cv::Scalar color = Colors::Black,
                          cv::Scalar bgColor = Colors::LightGray,
                          double scale = Consts::DEFAULT_FONT_SCALE,
                          int thickness = Consts::DEFAULT_FONT_THICKNESS,
                          uchar alpha = 80,
                          int fontFace = Consts::DEFAULT_FONT);

    /**
     * @brief enableStatusMsg enables the bottom left text area for auto status messages
     * 
     * During enable of this feature you can overide some default values. It is safe to
     * call this method again just to change display settings.
     * @param color is font color
     * @param bgColor is rect bg color
     * @param scale is font scale
     * @param thickness is font thickness
     * @param alpha is the alpha value of the rect bg [0,255] => [transparent,opaque]
     * @param fontFace is the OpenCV fonr to use
     */
    void enableStatusMsg(cv::Scalar color = Colors::Orange,
                         cv::Scalar bgColor = Colors::LightGray,
                         double scale = Consts::DEFAULT_FONT_SCALE,
                         int thickness = Consts::DEFAULT_FONT_THICKNESS,
                         uchar alpha = 80,
                         int fontFace = Consts::DEFAULT_FONT);

    /// set the default status message (active widget and shapes will override it)
    void setDefaultStatusMsg(const std::string &msg);

    /// get the default status message (active widget and shapes will override it)
    std::string getDefaultStatusMsg() const;

    /// manually set the screen text message. It remains until disabled or changed.
    void setScreenText(const std::string &msg);

    /// set the Canvas size to match the cv::Mat we'll use in redrawOn;
    void setSize(const cv::Size &value);

    /// get the Canvas size
    cv::Size getSize();

    virtual void addWidget(const std::shared_ptr<Widget> &widget);
    virtual std::shared_ptr<Widget> rmvWidget(const std::shared_ptr<Widget> &widget);

    /// is redrawOn() on/off?
    bool getOn() const;

    /// redrawOn will do nothing if value is 'false'
    void setOn(bool value);

    /// write all the shapes currently in the Canvas to a file
    void writeShapesToFile(const std::string &filepath) const;

    /// load all the from a file into the canvas (removing all current shapes in the process)
    void readShapesFromFile(const std::string &filepath);

    /// utility method to handle mouse events on the associated window
    void setMouseCallback();

    /// utility method which uses the winName encapsulated in Canvas
    void imshow(InputArray mat);

    /**
     * @brief waitKeyEx
     * 
     * utility method to handle key strokes - you get the keystroke if a widget/shape didn't consume it.
     * @param delay in milliseconds. If delay is 0, then the latest image used as input will be the
     * background for internal updates. If you want to change it from a callback, then use setImage.
     * @return the key press or -1 if the timeout reached
     * @sa setImage
     * @note
     * When using widgets with callback on the same image, the delay should be 0.
     * If you're changing frames or using only the polling API of the widgets, then
     * specify a delay of your choice.
     */
    int waitKeyEx(int delay = 0);

    /**
     * @brief applyTheme
     * 
     * apply the current theme to all existing widgets and shapes in the canvas
     * @param applyToCanvasText should be usually false (make it true to affect Status&UserText of Canvas).
     */
    void applyTheme(bool applyToCanvasText = false);

    /**
     * @brief fatal
     * 
     * A more elegant way for your app to exit on failures.
     * - A dedicated opencv window with a MsgBox wil show your message.
     * - The error code will be used with _Exit()
     * - The errorMsg will also be written to the standard error
     * @param errorMsg will be displayed to the user
     * @param exitStatus will be used with _Exit()
     */
    static void fatal(string errorMsg, int exitStatus);

    /// while waiting for events the Canvas is redrawn only if it is dirty
    void setDirty();

protected:
    virtual void recalc() {}

    virtual std::shared_ptr<Widget> rmvWidget(Widget *widget);


    virtual const cv::Rect getBoundaries() const;

private:

    Canvas();

    class StatusMsgGrd
    {
    public:
        StatusMsgGrd(Canvas &val) : c(val) {}
        ~StatusMsgGrd()
        {
            if (c.activeWidget)
            {
                c.setStatusMsg(c.activeWidget->getStatusMsg());
            }
            else if (c.activeShape)
            {
                c.setStatusMsg(c.activeShape->getStatusMsg());
            }
            else
            {
                c.setStatusMsg(c.defaultStatusMsg);
            }
        }
    private:
        Canvas &c;
    };
    friend class StatusMsgGrd;

    /**
     * @brief consumeKey takes a key value and tries to use it in a shape or widget
     * 
     * @param key is the key value received from the user.
     * It will change to -1 if it was consumed by a shape or widget.
     */
    void consumeKey(int &key);

    void setStatusMsg(const std::string &msg);

    virtual bool setDirtyLayout();

    void broadcastCreate(Shape *shape);
    void broadcastModify(Shape *shape);
    void broadcastDelete(Shape *shape);

    void processNewShape();

    bool on;
    bool isDirty;
    cv::Rect boundaries;
    bool hasScreenText;
    bool hasStatusMsg;
    std::shared_ptr<Text> screenText;
    std::shared_ptr<Text> statusMsg;
    std::string defaultStatusMsg;
    std::string shapeType;
    cv::Point dragPos;
    cv::Mat latestFrameSrc;
    std::string winName;
    std::list<std::shared_ptr<Shape>> shapes;
    std::list<std::shared_ptr<Widget>> widgets;
    std::shared_ptr<Shape> activeShape;
    std::shared_ptr<Widget> activeWidget;
    std::list<CBCanvasShape> createNotifs;
    std::list<CBCanvasShape> modifyNotifs;
    std::list<CBCanvasShape> deleteNotifs;

    friend void operator >> (const cv::FileNode& n, Canvas& value)
    {
        read( n, value, Canvas());
    }

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
    ((Shape*)shape.get())->setLocked(false); // (Maybe better is setReady does this for the needed shape)
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

#define CCV_STR(X) ((std::stringstream&)(std::stringstream() << X)).str()
#define CCV_C_STR(X) CCV_STR(X).c_str()

/** @example example_shapes_widgets.cpp
 * This is an example of using shapes and widgets together.
 */

#endif // CANVAS_H
