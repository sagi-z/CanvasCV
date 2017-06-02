#include "canvas.h"
#include "colors.h"
#include "shapefactory.h"
#include "shapesconnector.h"
#include <algorithm>

namespace canvascv
{

Canvas::Canvas(Size sizeVal)
    : size(sizeVal),
      hasScreenText(false),
      hasStatusMsg(false),
      screenText(Point(5,5)),
      statusMsg(Point(0,0)) // will be moved during enableStatusMsg
{
    screenText.setCanvas(*this);
    statusMsg.setCanvas(*this);
    statusMsg.setFlowDirection(FloatingText::BOTTOM_UP);
}

Canvas::~Canvas()
{
    clear();
}

void Canvas::redrawOn(const cv::Mat &src, cv::Mat &dst)
{
    if (&src != &dst)
    {
        dst = src.clone();
    }
    for (auto &shape : shapes)
    {
        if (shape->getVisible())
        {
            shape->draw(dst);
        }
    }
    for (auto &widget : widgets)
    {
        if (widget->getVisible())
        {
            widget->draw(dst);
        }
    }
    if (hasScreenText)
    {
        screenText.draw(dst);
    }
    if (hasStatusMsg)
    {
        // reserve ~ 2 lines for the status msg at screen bottom left
        statusMsg.setLeftPos(Point(5, dst.rows - statusMsg.getFontHeight()));
        statusMsg.draw(dst);
    }
}

void Canvas::onMousePress(const cv::Point &pos)
{
    // widgets have preference over shapes
    if (activeWidget.get())
    {
        if (activeWidget->isAtPos(pos))
        {
            activeWidget->broadcastChange(Widget::PRESS);
            return;
        }
        else
        {
            activeWidget->broadcastChange(Widget::LEAVE);
            setStatusMsg("");
            activeWidget.reset();
        }
    }

    // delegate to active
    if (active.get())
    {
        if (! active->mousePressed(pos))
        {
            active->lostFocus();
            active->broadcastSelectChange(false);
            broadcastModify(active.get());
            active.reset();
            setStatusMsg("");
        }
        return;
    }

    // try to set active
    for (auto &shape : shapes)
    {
        if (shape->mousePressed(pos))
        {
            active = shape;
            if (hasStatusMsg)
            {
                if (active->getLocked())
                {
                    setStatusMsg("Shape is locked.");
                }
                else
                {
                    setStatusMsg(active->getStatusMsg());
                }
            }
            active->broadcastSelectChange(true);
            return;
        }
    }

    // create a new shape and set it as active
    shapes.push_back(std::shared_ptr<Shape>(ShapeFactory::newShape(shapeType,pos)));
    processNewShape();
    if (! active->mousePressed(pos, true))
    {
        active->lostFocus();
        active.reset();
        setStatusMsg("");
    }
    else
    {
        active->broadcastSelectChange(true);
    }
}

void Canvas::onMouseRelease(const cv::Point &pos)
{
    // widgets have preference over shapes
    if (activeWidget.get())
    {
        if (activeWidget->isAtPos(pos))
        {
            activeWidget->broadcastChange(Widget::RELEASE);
            return;
        }
        else
        {
            activeWidget->broadcastChange(Widget::LEAVE);
            setStatusMsg("");
            activeWidget.reset();
        }
    }

    if (active.get())
    {
        if (! active->mouseReleased(pos))
        {
            active->lostFocus();
            active->broadcastSelectChange(false);
            broadcastModify(active.get());
            active.reset();
            setStatusMsg("");
        }
    }
}

void Canvas::onMouseMove(const cv::Point &pos)
{
    // widgets have preference over shapes
    if (activeWidget.get())
    {
        if (! activeWidget->isAtPos(pos))
        {
            activeWidget->broadcastChange(Widget::LEAVE);
            setStatusMsg("");
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
            setStatusMsg(widget->getStatusMsg());
            return;
        }
    }

    // shapes
    if (active.get())
    {
        active->mouseMoved(pos);
    }
}

std::shared_ptr<Shape> Canvas::createShape(const Point &pos, string type)
{
    shapes.push_back(std::shared_ptr<Shape>(ShapeFactory::newShape(type,pos)));
    processNewShape();
    active->lostFocus();
    return active;
}

std::shared_ptr<Widget> Canvas::createWidget(const Point &pos, string type)
{
    std::shared_ptr<Widget> widget(WidgetFactory::newWidget(type, pos));
    widget->setCanvas(*this);
    widgets.push_back(widget);
    return widget;
}

void Canvas::consumeKey(int &key)
{
    if (key != -1)
    {
        if (active.get())
        {
            if (! active->keyPressed(key))
            {
                active->lostFocus();
                active->broadcastSelectChange(false);
                active.reset();
                setStatusMsg("");
            }
        }
    }
}

void Canvas::deleteActive()
{
    if (active.get())
    {
        deleteShape(active);
        active.reset();
        setStatusMsg("");
    }
}

void Canvas::deleteShape(std::shared_ptr<Shape> shape)
{
    shapes.erase(find(shapes.begin(),shapes.end(),shape));
    shape->lostFocus();
    shape->broadcastSelectChange(false);
    broadcastDelete(shape.get());
    std::list<std::shared_ptr<ShapesConnector>> connectors;
    getShapes(connectors);
    for (auto &connector : connectors)
    {
        connector->disconnectShape(shape->getId());
    }
}

void Canvas::deleteWidget(std::shared_ptr<Widget> widget)
{
    widgets.erase(find(widgets.begin(),widgets.end(),widget));
}

void Canvas::notifyOnCreate(Canvas::CBType cb)
{
    createNotifs.push_back(cb);
}

void Canvas::notifyOnModify(Canvas::CBType cb)
{
    modifyNotifs.push_back(cb);
}

void Canvas::notifyOnDelete(Canvas::CBType cb)
{
    deleteNotifs.push_back(cb);
}

void Canvas::clear()
{
    active.reset();
    setStatusMsg("");
    for (auto &shape : shapes)
    {
        broadcastDelete(shape.get());
    }
    shapes.clear();
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

void Canvas::enableScreenText(Scalar color, Scalar bgColor, double scale, int thickness, double alpha, int fontFace)
{
    hasScreenText = true;
    screenText.setOutlineColor(color);
    screenText.setFillColor(bgColor);
    screenText.setFontScale(scale);
    screenText.setAlpha(alpha);
    screenText.setFontFace(fontFace);
}

void Canvas::enableStatusMsg(Scalar color, Scalar bgColor, double scale, int thickness, double alpha, int fontFace)
{
    hasStatusMsg = true;
    statusMsg.setOutlineColor(color);
    statusMsg.setFillColor(bgColor);
    statusMsg.setFontScale(scale);
    statusMsg.setAlpha(alpha);
    statusMsg.setFontFace(fontFace);
}

void Canvas::setStatusMsg(const string &msg)
{
    if (hasStatusMsg)
    {
        statusMsg.setMsg(msg);
    }
}

void Canvas::setScreenText(const string &msg)
{
    if (hasScreenText)
    {
        screenText.setMsg(msg);
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
    active = shapes.back();
    active->setCanvas(*this);
    broadcastCreate(active.get());
    if (hasStatusMsg)
    {
        if (active->getLocked())
        {
            setStatusMsg("Shape is locked.");
        }
        else
        {
            setStatusMsg(active->getStatusMsg());
        }
    }
}

cv::Size Canvas::getSize() const
{
    return size;
}

void Canvas::setSize(const cv::Size &value)
{
    size = value;
    for (auto &widget : widgets)
    {
        widget->canvasResized(value);
    }
}

void write(cv::FileStorage& fs, const std::string&, const Canvas& x)
{
    if (x.active.get())
    {
    }
    fs << "{";
    fs << "shapes" << "[";
    for (auto &shape : x.shapes)
    {
        fs << *shape;
    }
    fs << "]";
    fs << "}";
    if (x.active.get()) {
    }
}

void read(const cv::FileNode& node, Canvas& x, const Canvas&)
{
    x.clear();
    cv::FileNode n = node["shapes"];
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

}
