#ifndef SHAPE_H
#define SHAPE_H

#include "colors.h"

#include <opencv2/core/mat.hpp>
#include <string>
#include <list>
#include <iostream>
#include <memory>

namespace canvascv
{

class Handle;
class Canvas;

class Shape
{
public:

    Shape()
        : id(genId()),
          outlineColor(Colors::GREEN),
          fillColor(outlineColor),
          locked(false),
          visible(true),
          editing(false),
          thickness(1),
          lineType(cv::LINE_AA),
          canvas(nullptr)
    {}

    Shape(const Shape &other)
        : id(genId()),
          outlineColor(other.outlineColor),
          fillColor(other.fillColor),
          locked(other.locked),
          visible(other.visible),
          editing(other.editing),
          thickness(other.thickness),
          lineType(other.lineType),
          canvas(other.canvas)
    {}

    virtual ~Shape();

    /**
     * @brief draw shape on the canvas
     * @param canvas
     */
    virtual void draw(cv::Mat &canvas) = 0;

    /**
     * @brief mousePressed
     * @param pos
     * @param onCreate is true if this is the mouse press which cerated this shape
     * @return true for keep in focus, false for leave focus
     */
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false) = 0;

    /**
     * @brief mouseMoved
     *
     * 1. Was a mouse moved over this shape?
     * 2. If shape is during edit, then these are the mouse position.
     * @param pos
     * @return true if a mouse moved over this shape, or it is during edit. false otherwise.
     */
    virtual bool mouseMoved(const cv::Point &pos) = 0;

    /**
     * @brief getConnectionTargets
     * Return a list of Handles this shape allows to connect to from other shapes - mainly from ShapesConnector
     * @return list of Handle pointers we ShapesConnector can use to connect
     */
    virtual std::list<Handle*> getConnectionTargets() = 0;

    /**
     * @brief getShape
     * Get internal shapes, which Canvas doesn't know of.
     * @param id
     * @return internal sub shape with requested id
     */
    virtual std::shared_ptr<Shape> getShape(int id)
    {
        return nullptr;
    }

    /**
     * @brief keyPressed will be called by Canvas for active shapes
     * @param key was pressed. You must set it to -1 if you consumed it.
     * @return true if we want to stay in focus and false otherwise
     */
    virtual bool keyPressed(int &key)
    {
        (void)key; // remove compile warning;
        // consume nothing and keep focus for any key press by default
        return true;
    }

    /**
     * @brief lostFocus is called by Canvas if we're in it and just became non-active
     */
    virtual void lostFocus()
    {
        // do nothing by default
    }

    /**
     * @brief mouseReleased
     * @param pos
     * @return true for keep in focus, false for leave focus
     */
    virtual bool mouseReleased(const cv::Point &pos) = 0;

    virtual bool isAtPos(const cv::Point &pos) = 0;

    cv::Scalar getOutlineColor() const
    {
        return outlineColor;
    }

    virtual void setOutlineColor(const cv::Scalar &value)
    {
        outlineColor = value;
    }

    cv::Scalar getFillColor() const
    {
        return fillColor;
    }

    virtual void setFillColor(const cv::Scalar &value)
    {
        fillColor = value;
    }

    bool getLocked() const
    {
        return locked;
    }

    virtual void setLocked(bool value)
    {
        locked = value;
    }

    bool getVisible() const
    {
        return visible;
    }

    virtual void setVisible(bool value)
    {
        visible = value;
    }

    bool isEditing()
    {
        return editing;
    }

    /**
     * @brief getType should be implemented by derived.
     * @return std::string of type
     */
    virtual const char *getType() const = 0;

    int getThickness() const
    {
        return thickness;
    }

    virtual void setThickness(int value)
    {
        thickness = value;
    }

    int getLineType() const
    {
        return lineType;
    }

    virtual void setLineType(int value)
    {
        lineType = value;
    }

    int getId()
    {
        return id;
    }

    /**
     * @brief setCanvas
     * Called when canvas is creating a shape.
     * Only realy used by specific shapes.
     * @param value
     */
    void setCanvas(Canvas &value);

    virtual const std::string &getStatusMsg() const;

protected:
    virtual void writeInternals(cv::FileStorage& fs) const = 0;
    virtual void readInternals(const cv::FileNode& node) = 0;

    int id;
    cv::Scalar outlineColor;
    cv::Scalar fillColor;
    bool locked;
    bool visible;
    bool editing;
    int thickness;
    int lineType;
    Canvas *canvas;

private:
    int genId()
    {
        static int idGenerator;
        return ++idGenerator;
    }

    friend void write(cv::FileStorage& fs, const std::string&, const Shape& x);
    friend void read(const cv::FileNode& node, Shape*& x, const Shape* default_value);

    void write(cv::FileStorage& fs) const;
    void read(const cv::FileNode& node);
};

// These write and read functions must be defined for the serialization in cv::FileStorage to work
void write(cv::FileStorage& fs, const std::string&, const Shape& x);
void read(const cv::FileNode& node, Shape*& x, const Shape* default_value);

// Will write the xml to memory and output it's std::string
std::ostream & operator<<(std::ostream& o, const Shape &shape);

}

#endif // SHAPE_H
