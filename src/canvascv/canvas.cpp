#include "canvas.h"
#include "colors.h"
#include "shapes/shapefactory.h"
#include "shapes/shape.h"
#include "shapes/shapesconnector.h"
#include "themes/themerepository.h"
#include "themes/theme.h"

#include "widgets/msgbox.h"

#include <algorithm>
#include <cstdlib>

using namespace std;
using namespace cv;

namespace canvascv
{

Canvas::Canvas(const string &winNameVal, Size sizeVal)
    : on(true),
      isDirty(false),
      boundaries(Point(0,0), sizeVal),
      hasScreenText(false),
      hasStatusMsg(false),
      dragPos(0,0),
      winName(winNameVal)
{
    if (sizeVal.width && sizeVal.height)
    {
        latestFrameSrc.create(sizeVal, CV_8UC3);
        latestFrameSrc = Colors::White;
    }
}

Canvas::~Canvas()
{
    clearShapes();
}

void Canvas::redrawOn(const Mat &src, Mat &dst)
{
    if (&src != &dst)
    {
        dst.create(src.size(), src.type());
        src.copyTo(dst);
    }
    if (! on) return;

    latestFrameSrc = src;

    if (src.channels() == 1)
    {
        cv::cvtColor(src, dst, CV_GRAY2BGR);
    }

    for (auto &shape : shapes)
    {
        if (shape->getVisible())
        {
            shape->draw(dst);
        }
    }

    // Updating dirty widgets before drawing them
    updateDirtyWidgets();

    // widgets are drawn on top of shapes
    for (auto &widget : widgets)
    {
        if (widget->getVisible())
        {
            widget->renderOn(dst);
        }
    }

    // These go on top of everything
    if (hasScreenText)
    {
        static_cast<Widget*>(screenText.get())->renderOn(dst);
    }
    if (hasStatusMsg)
    {
        statusMsg->setLocation(Point(5, dst.rows - 5));
        static_cast<Widget*>(statusMsg.get())->renderOn(dst);
    }

    isDirty = false;
}

void Canvas::redrawOn(Mat &dst)
{
    redrawOn(latestFrameSrc, dst);
}

void Canvas::setImage(const Mat &img)
{
    latestFrameSrc = img;
    setSize(img.size());
    isDirty = true;
}

bool Canvas::onMousePress(const Point &pos)
{
    if (! on) return false;
    isDirty = true;
    StatusMsgGrd(*this);

    // widgets have preference over shapes
    if (activeWidget.get())
    {
        if (activeWidget->isAtPos(pos))
        {
            activeWidget->broadcastChange(Widget::PRESS);
            return true;
        }
        else
        {
            activeWidget->broadcastChange(Widget::LEAVE);
            activeWidget.reset();
        }
    }

    // delegate to active shape
    if (activeShape.get())
    {
        bool wasReady = activeShape->isReady();
        if (! activeShape->mousePressed(pos))
        {
            activeShape->lostFocus();
            activeShape->broadcastEvent(Shape::UNSELECT);
            broadcastModify(activeShape.get());
            if (activeShape->isDeleted())
            {
                deleteActive();
            }
            else
            {
                activeShape.reset();
            }
        }
        else if (activeShape->isReady() && activeShape->getVisible() && ! activeShape->getLocked())
        {   // note that only active shapes can be dragged
            dragPos = pos;
        }
        if (! wasReady && activeShape && activeShape->isReady())
        {
            broadcastCreate(activeShape.get());
        }
        return false;
    }

    // try to set active shape
    for (auto &shape : shapes)
    {
        if (shape->mousePressed(pos))
        {
            activeShape = shape;
            activeShape->broadcastEvent(Shape::SELECT);
            return true;
        }
    }

    // create a new shape and set it as active
    if (shapeType.length())
    {
        shapes.push_back(std::shared_ptr<Shape>(ShapeFactory::newShape(shapeType,pos)));
        processNewShape();
        if (! activeShape->mousePressed(pos, true))
        {
            activeShape->lostFocus();
            if (activeShape->isDeleted())
            {
                deleteActive();
            }
            else
            {
                activeShape.reset();
            }
        }
        else
        {
            activeShape->broadcastEvent(Shape::SELECT);
            return true;
        }
    }

    return false;
}

void Canvas::onMouseRelease(const Point &pos)
{
    if (! on) return;
    isDirty = true;
    StatusMsgGrd(*this);

    dragPos.x = dragPos.y = 0;

    // widgets have preference over shapes
    if (activeWidget.get())
    {
        if (activeWidget->isAtPos(pos))
        {
            activeWidget->broadcastChange(Widget::RELEASE);
            activeWidget->broadcastChange(Widget::ENTER);
            return;
        }
        else
        {
            activeWidget->broadcastChange(Widget::LEAVE);
            activeWidget.reset();
        }
    }

    if (activeShape.get())
    {
        bool wasReady = activeShape->isReady();
        if (! activeShape->mouseReleased(pos))
        {
            activeShape->lostFocus();
            activeShape->broadcastEvent(Shape::UNSELECT);
            broadcastModify(activeShape.get());
            if (activeShape->isDeleted())
            {
                deleteActive();
            }
            else
            {
                activeShape.reset();
            }
        }
        if (! wasReady && activeShape && activeShape->isReady())
        {
            broadcastCreate(activeShape.get());
        }
    }
}

void Canvas::onMouseMove(const Point &pos)
{
    if (! on) return;
    isDirty = true;
    StatusMsgGrd(*this);

    // widgets have preference over shapes
    if (activeWidget.get())
    {
        if (! activeWidget->isAtPos(pos))
        {
            activeWidget->broadcastChange(Widget::LEAVE);
            activeWidget.reset();
        }
        else
        {
            return;
        }
    }

    // try to set active widget
    for (auto &widget : widgets)
    {
        if (widget->getVisible() && widget->isAtPos(pos))
        {
            activeWidget = widget;
            activeWidget->broadcastChange(Widget::ENTER);
            return;
        }
    }

    // shapes can be dragged
    if (activeShape.get())
    {
        if (dragPos.x || dragPos.y)
        {
            Point offset = pos - dragPos;
            activeShape->translate(offset);
            dragPos = pos;
            broadcastModify(activeShape.get());
        }
        else
        {
            activeShape->mouseMoved(pos);
        }
    }
}

std::shared_ptr<Shape> Canvas::createShape(string type, const Point &pos)
{
    std::shared_ptr<Shape> shape(ShapeFactory::newShape(type, pos));
    shapes.push_back(shape);
    processNewShape();
    shape->setReady();
    shape->setLocked(false); // (Maybe better is setReady does this for the needed shape)
    shape->lostFocus();
    activeShape.reset();
    return shape;
}

void Canvas::consumeKey(int &key)
{
    if (! on) return;
    StatusMsgGrd(*this);

    if (key != -1)
    {
        if (activeShape.get())
        {
            isDirty = true;
            bool wasReady = activeShape->isReady();
            if (! activeShape->keyPressed(key))
            {
                activeShape->lostFocus();
                activeShape->broadcastEvent(Shape::UNSELECT);
                if (activeShape->isDeleted())
                {
                    deleteActive();
                }
                else
                {
                    activeShape.reset();
                }
            }
            if (! wasReady && activeShape && activeShape->isReady())
            {
                broadcastCreate(activeShape.get());
            }
        }
    }
}

void Canvas::deleteActive()
{
    StatusMsgGrd(*this);
    if (activeShape.get())
    {
        deleteShape(activeShape);
        activeShape->lostFocus();
        activeShape.reset();
    }
}

void Canvas::deleteShape(const std::shared_ptr<Shape> &shape)
{
    shape->broadcastEvent(Shape::REMOVED);
    broadcastDelete(shape.get());
    std::list<std::shared_ptr<ShapesConnector>> connectors;
    getShapes(connectors);
    for (auto &connector : connectors)
    {
        connector->disconnectShape(shape->getId());
    }
    shapes.erase(find(shapes.begin(),shapes.end(),shape));
    isDirty = true;
}

void Canvas::deleteWidget(const std::shared_ptr<Widget> &widget)
{
    widgets.erase(find(widgets.begin(),widgets.end(),widget));
    isDirty = true;
}

void Canvas::notifyOnShapeCreate(Canvas::CBCanvasShape cb)
{
    createNotifs.push_back(cb);
}

void Canvas::notifyOnShapeModify(Canvas::CBCanvasShape cb)
{
    modifyNotifs.push_back(cb);
}

void Canvas::notifyOnShapeDelete(Canvas::CBCanvasShape cb)
{
    deleteNotifs.push_back(cb);
}

void Canvas::clearShapes()
{
    deleteActive();
    setStatusMsg(defaultStatusMsg);
    while(shapes.size())
    {
       deleteShape(shapes.back());
    }
}

void Canvas::clearWidgets()
{
    setStatusMsg(defaultStatusMsg);
    while(widgets.size())
    {
       rmvWidget(widgets.back());
    }
}

std::shared_ptr<Shape> Canvas::getShape(int id)
{
    for (auto &shape : shapes)
    {
        if (shape->getId() == id)
        {
            return shape;
        }
        std::shared_ptr<Shape> subShape = shape->getShape(id);
        if (subShape.get())
        {
            return subShape;
        }
    }
    return nullptr;
}

void Canvas::getShapes(const Point &pos, std::list<std::shared_ptr<Shape> > &result)
{
    result.clear();
    for (auto &shape : shapes)
    {
        if (shape->isAtPos(pos))
        {
            result.push_back(shape);
        }
    }
}

void Canvas::enableScreenText(Scalar color, Scalar bgColor, double scale, int thickness, uchar alpha, int fontFace)
{
    hasScreenText = true;
    if (! screenText.get())
    {
        screenText = Text::create(*this, Point(5,5));
        screenText->setMaxWidth(boundaries.width);
    }
    screenText->setOutlineColor(color);
    screenText->setFillColor(bgColor);
    screenText->setFontScale(scale);
    screenText->setThickness(thickness);
    screenText->setAlpha(alpha);
    screenText->setFontFace(fontFace);
}

void Canvas::enableStatusMsg(Scalar color, Scalar bgColor, double scale, int thickness, uchar alpha, int fontFace)
{
    hasStatusMsg = true;
    if (! statusMsg.get())
    {
        statusMsg = Text::create(*this, Point(0,0));  // location, size dependent, is set during redrawOn
        statusMsg->setMaxWidth(boundaries.width);
        statusMsg->setFlowAnchor(Widget::BOTTOM_LEFT);
    }
    statusMsg->setOutlineColor(color);
    statusMsg->setFillColor(bgColor);
    statusMsg->setFontScale(scale);
    statusMsg->setThickness(thickness);
    statusMsg->setAlpha(alpha);
    statusMsg->setFontFace(fontFace);
}

void Canvas::setDefaultStatusMsg(const string &msg)
{
   defaultStatusMsg = msg;
   setStatusMsg(defaultStatusMsg);
}

void Canvas::setStatusMsg(const string &msg)
{
    if (hasStatusMsg)
    {
        statusMsg->setText(msg);
    }
}

void Canvas::setScreenText(const string &msg)
{
    if (hasScreenText)
    {
        screenText->setText(msg);
    }
}

void Canvas::broadcastCreate(Shape *shape)
{
    for (auto &cb : createNotifs)
    {
        cb(shape);
    }
}

void Canvas::broadcastModify(Shape *shape)
{
    for (auto &cb : modifyNotifs)
    {
        cb(shape);
    }
}

void Canvas::broadcastDelete(Shape *shape)
{
    for (auto &cb : deleteNotifs)
    {
        cb(shape);
    }
}

void Canvas::processNewShape()
{
    StatusMsgGrd(*this);
    activeShape = shapes.back();
    activeShape->setCanvas(*this);
    if (activeShape->isReady()) broadcastCreate(activeShape.get());
    isDirty = true;
}

std::string Canvas::getDefaultStatusMsg() const
{
    return defaultStatusMsg;
}

bool Canvas::getOn() const
{
    return on;
}

void Canvas::setOn(bool value)
{
    on = value;
}

void Canvas::writeShapesToFile(const string &filepath) const
{
    FileStorage fs(filepath, FileStorage::WRITE);
    fs << "CanvasShapes" << *this;
}

void Canvas::readShapesFromFile(const string &filepath)
{
    FileStorage fs(filepath, FileStorage::READ);
    fs["CanvasShapes"] >> *this;
}


static void mouseCB(int event, int x, int y, int flags, void* userData) {
    (void)flags;
    Canvas *pCanvas=reinterpret_cast<Canvas*>(userData);
    switch( event )
    {
    case EVENT_LBUTTONDOWN:
        pCanvas->onMousePress(Point(x,y));
        break;
    case EVENT_LBUTTONUP:
        pCanvas->onMouseRelease(Point(x,y));
        break;
    case EVENT_MOUSEMOVE:
        pCanvas->onMouseMove(Point(x,y));
        break;
    }
}

void Canvas::setMouseCallback()
{
    cv::setMouseCallback(winName, mouseCB, this);
}

void Canvas::imshow(InputArray mat)
{
   cv::imshow(winName, mat);
}

int Canvas::waitKeyEx(int delay)
{
    bool delayZero = false;
    if (delay <= 0)
    {
        delayZero = true;
        delay = 1000/25.; // ~ 25 FPS
    }
    int key = -1;
    auto start = std::chrono::high_resolution_clock::now();
    Mat internalOut;
    do
    {
#if OPENCV_HAS_WAITKEYEX
        key = cv::waitKeyEx(delay);
#else
        key = cv::waitKey(delay);
#endif
        consumeKey(key);
        if (! delayZero)
        {   // check timeout on delay
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> diff = end-start;
            if (diff.count() >= delay) break;
        }
        if (key == -1)
        {
            if (on && (isDirty || hasDirtyWidgets()))
            {
                redrawOn(internalOut);
                imshow(internalOut);
            }
        }
        else
        {
            break;
        }
    } while (true);
    return key;
}

void Canvas::applyTheme(bool applyToCanvasText)
{
    isDirty = true;
    Theme *currentTheme = ThemeRepository::getCurrentTheme();
    for (auto &shape : shapes)
    {
        currentTheme->applyStyle(shape.get());
    }

    for (auto &widget : widgets)
    {
        if (applyToCanvasText || (widget.get() != screenText.get() &&
                                  widget.get() != statusMsg.get()))
        {
            currentTheme->applyStyle(widget.get());
        }
    }
}

void Canvas::fatal(string errorMsg, int exitStatus)
{
    cerr << errorMsg << endl;
    string header = "Fatal Error:\n";
    MsgBox::createModal("Fatal Error", header + errorMsg , {"Exit"}, [exitStatus](Widget*,int) {_Exit(exitStatus);});
}

void Canvas::setDirty()
{
    isDirty = true;
}

const Rect Canvas::getBoundaries() const
{
    return boundaries;
}

Canvas::Canvas()
    : on(true),
      isDirty(false),
      hasScreenText(false),
      hasStatusMsg(false),
      dragPos(0,0)
{
}

void Canvas::setSize(const Size &value)
{
    if (boundaries.size() != value)
    {
        boundaries.width = value.width;
        boundaries.height = value.height;
        if (latestFrameSrc.size() != value)
        {
            latestFrameSrc.create(value, CV_8UC3);
            latestFrameSrc = Colors::White;
        }
        for (auto &widget : widgets)
        {
            widget->layoutResized(boundaries);
        }
    }
}

Size Canvas::getSize()
{
   return boundaries.size();
}

void write(FileStorage& fs, const std::string&, const Canvas& x)
{
    if (x.activeShape.get())
    {
    }
    fs << "{";
    fs << "winName" << x.winName;
    fs << "shapes" << "[";
    for (auto &shape : x.shapes)
    {
        if (shape->isReady())
        {
            fs << *shape;
        }
    }
    fs << "]";
    fs << "}";
    if (x.activeShape.get()) {
    }
}

void read(const FileNode& node, Canvas& x, const Canvas&)
{
    x.clearShapes();
    node["winName"] >> x.winName;
    FileNode n = node["shapes"];
    FileNodeIterator it = n.begin(), it_end = n.end();
    for (; it != it_end; )
    { // ++it is done automatically by "it >> shape"
        Shape *shape = 0;
        it >> shape;
        assert(shape != 0);
        x.shapes.push_back(std::shared_ptr<Shape>(shape));
        shape->lostFocus();
        shape->setCanvas(x);
    }
    std::list<std::shared_ptr<ShapesConnector>> connectors;
    x.getShapes(connectors);
    for (auto &connector : connectors)
    {
        connector->reconnect();
    }
    for (auto &shape : x.shapes)
    {
        x.broadcastCreate(shape.get());
    }
}

shared_ptr<Widget> Canvas::rmvWidget(Widget *widget)
{
    shared_ptr<Widget> result;
    auto i = find_if(widgets.begin(),
                     widgets.end(),
                     [widget](const shared_ptr<Widget> &item)->bool
    {
        return item.get() == widget;
    });
    if (i != widgets.end())
    {
        result = *i;
        widgets.erase(i);
        rmvDirtyWidget(widget);
        if (widget == activeWidget.get())
        {
            activeWidget.reset();
        }
        isDirty = true;
    }
    return result;
}

void Canvas::addWidget(const shared_ptr<Widget> &widget)
{
    widget->rmvFromLayout();
    widget->setLayout(*this);
    widgets.push_back(widget);
    isDirty = true;
}

shared_ptr<Widget> Canvas::rmvWidget(const shared_ptr<Widget> &widget)
{
    return rmvWidget(widget.get());
}

bool Canvas::setDirtyLayout()
{
    isDirty = true;
    return true; // we'll always try to updateDirtyWidgets
}

}
