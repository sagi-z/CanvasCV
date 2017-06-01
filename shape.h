#ifndef SHAPE_H
#define SHAPE_H

#include "colors.h"

#include <opencv2/core/mat.hpp>
#include <string>
#include <list>
#include <iostream>
#include <memory>
#include <functional>

namespace canvascv
{

class Handle;
class Canvas;

/**
 * @brief The Shape class
 * Code from outside of this namespace should only use members marked as PUBLIC.
 * INTERNAL api usage might cause instability and compatibility issues.
 */
class Shape
{
public:
    // your cb is called with the shape pointer and a 'true'/'false' for selected/unselected,
    typedef std::function<void(Shape*, bool)> CBType;

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
     * @brief PUBLIC: used to register for notifications on shape seletion by user
     * @param cb to invoke on shape creation
     */
    void notifyOnSelect(CBType cb);

    /**
     * @brief INTERNAL: draw shape on the canvas
     * @param canvas
     */
    virtual void draw(cv::Mat &canvas) = 0;

    /**
     * @brief INTERNAL: mousePressed
     * @param pos
     * @param onCreate is true if this is the mouse press which cerated this shape
     * @return true for keep in focus, false for leave focus
     */
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false) = 0;

    /**
     * @brief INTERNAL: mouseMoved
     *
     * 1. Was a mouse moved over this shape?
     * 2. If shape is during edit, then these are the mouse position.
     * @param pos
     * @return true if a mouse moved over this shape, or it is during edit. false otherwise.
     */
    virtual bool mouseMoved(const cv::Point &pos) = 0;

    /**
     * @brief PUBLIC: getConnectionTargets
     * Return a list of Handles this shape allows to connect to from other shapes - mainly from ShapesConnector
     * @return list of Handle pointers we ShapesConnector can use to connect
     */
    virtual std::list<Handle*> getConnectionTargets() = 0;

    /**
     * @brief INTERNAL: getShape
     * Get internal shapes, which Canvas doesn't know of.
     * @param id
     * @return internal sub shape with requested id
     */
    virtual std::shared_ptr<Shape> getShape(int id)
    {
        return nullptr;
    }

    /**
     * @brief INTERNAL: keyPressed will be called by Canvas for active shapes
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
     * @brief INTERNAL: lostFocus is called by Canvas if we're in it and just became non-active
     */
    virtual void lostFocus()
    {
        // do nothing by default
    }

    /**
     * @brief INTERNAL: mouseReleased
     * @param pos
     * @return true for keep in focus, false for leave focus
     */
    virtual bool mouseReleased(const cv::Point &pos) = 0;

    /// INTERNAL
    virtual bool isAtPos(const cv::Point &pos) = 0;

    /// PUBLIC
    cv::Scalar getOutlineColor() const
    {
        return outlineColor;
    }

    /// PUBLIC
    virtual void setOutlineColor(const cv::Scalar &value)
    {
        outlineColor = value;
    }

    /// PUBLIC
    cv::Scalar getFillColor() const
    {
        return fillColor;
    }

    /// PUBLIC
    virtual void setFillColor(const cv::Scalar &value)
    {
        fillColor = value;
    }

    /// PUBLIC
    bool getLocked() const
    {
        return locked;
    }

    /// PUBLIC
    virtual void setLocked(bool value)
    {
        locked = value;
    }

    /// PUBLIC
    bool getVisible() const
    {
        return visible;
    }

    /// PUBLIC
    virtual void setVisible(bool value)
    {
        visible = value;
    }

    /// INTERNAL
    bool isEditing()
    {
        return editing;
    }

    /**
     * @brief PUBLIC: getType should be implemented by derived.
     * @return std::string of type
     */
    virtual const char *getType() const = 0;

    /// PUBLIC
    int getThickness() const
    {
        return thickness;
    }

    /// PUBLIC
    virtual void setThickness(int value)
    {
        thickness = value;
    }

    /// PUBLIC
    int getLineType() const
    {
        return lineType;
    }

    /// PUBLIC
    virtual void setLineType(int value)
    {
        lineType = value;
    }

    /// PUBLIC
    int getId()
    {
        return id;
    }

    /**
     * @brief INTERNAL: setCanvas
     * Called when canvas is creating a shape.
     * Only realy used by specific shapes.
     * @param value
     */
    void setCanvas(Canvas &value);

    /// INTERNAL
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
    friend class Canvas;

    /// called by the canvas when the shape is (un)selected
    void broadcastSelectChange(bool selected);

    int genId()
    {
        static int idGenerator;
        return ++idGenerator;
    }

    friend void write(cv::FileStorage& fs, const std::string&, const Shape& x);
    friend void read(const cv::FileNode& node, Shape*& x, const Shape* default_value);

    void write(cv::FileStorage& fs) const;
    void read(const cv::FileNode& node);

    std::list<CBType> selectNotifs;
};

// These write and read functions must be defined for the serialization in cv::FileStorage to work
void write(cv::FileStorage& fs, const std::string&, const Shape& x);
void read(const cv::FileNode& node, Shape*& x, const Shape* default_value);

// Will write the xml to memory and output it's std::string
std::ostream & operator<<(std::ostream& o, const Shape &shape);

}

#endif // SHAPE_H
