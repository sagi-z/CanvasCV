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
 * @brief The Shape class hierarchy is for geomertric user interaction.
 * Use shapes to get on screen selections and landmark settings from your user.
 * @see ThemeRepository for using themes. The default theme will
 * overide some CTOR values.
 */
class Shape
{
public:

    /// constructor
    Shape()
        : id(genId()),
          outlineColor(Colors::Green),
          fillColor(outlineColor),
          locked(false),
          visible(true),
          editing(false),
          thickness(1),
          lineType(cv::LINE_AA),
          canvas(nullptr),
          deleted(false)
    {}

    /// copy constructor
    Shape(const Shape &other)
        : id(genId()),
          outlineColor(other.outlineColor),
          fillColor(other.fillColor),
          locked(other.locked),
          visible(other.visible),
          editing(other.editing),
          thickness(other.thickness),
          lineType(other.lineType),
          canvas(other.canvas),
          deleted(other.deleted)
    {}

    /// virtual destructor
    virtual ~Shape();

    /**
     * @brief The CBEvent enum will let you know what just happended to the shape
     */
    enum CBEvent
    {
        SELECT,   ///< shape selected
        UNSELECT, ///< shape unselected
        REMOVED   ///< shape removed
    };

    /// signature of a callback which gets the CBEvent
    typedef std::function<void(Shape*, CBEvent)> CBType;

    /**
     * @brief used to register for notifications on shape
     * @param cb to invoke on shape event
     */
    void notifyOnEvent(CBType cb);


    /**
     * @brief getConnectionTargets
     * Return a list of Handles this shape allows to connect to from other shapes (mainly for ShapesConnector)
     * @return list of Handle pointers we ShapesConnector can use to connect
     */
    virtual std::list<Handle*> getConnectionTargets() = 0;


    /// get the outline color
    cv::Scalar getOutlineColor() const
    {
        return outlineColor;
    }

    /// set the outline color
    virtual void setOutlineColor(const cv::Scalar &value)
    {
        outlineColor = value;
    }

    /// get the fill color (fill color is not very useful for shapes right now)
    cv::Scalar getFillColor() const
    {
        return fillColor;
    }

    /// set the fill color (fill color is not very useful for shapes right now)
    virtual void setFillColor(const cv::Scalar &value)
    {
        fillColor = value;
    }

    /// is the shape locked (can't be moved/edited)
    bool getLocked() const
    {
        return locked;
    }

    /// set the shape lock state (can/can't be moved/edited)
    virtual void setLocked(bool value)
    {
        locked = value;
    }

    /// is the shape visible
    bool getVisible() const
    {
        return visible;
    }

    /// set the shape visible state
    virtual void setVisible(bool value)
    {
        visible = value;
    }

    /**
     * @brief getType is always implemented by derived to return the same static pointer per shape.
     * @return const char * pointer to string with shape type name
     */
    virtual const char *getType() const = 0;

    /// get line thickness to use when drawing
    int getThickness() const
    {
        return thickness;
    }

    /// set line thickness to use when drawing
    virtual void setThickness(int value)
    {
        thickness = value;
    }

    /// get the line type (LINE_4, LINE_8, LINE_AA)
    int getLineType() const
    {
        return lineType;
    }

    /// set the line type (LINE_4, LINE_8, LINE_AA)
    virtual void setLineType(int value)
    {
        lineType = value;
    }

    /// returns true if shape is at pos, false otherwise
    virtual bool isAtPos(const cv::Point &pos) = 0;


    /// return a unique id for this shape
    int getId()
    {
        return id;
    }

protected:
    virtual void writeInternals(cv::FileStorage& fs) const = 0;
    virtual void readInternals(const cv::FileNode& node) = 0;

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

    bool isEditing()
    {
        return editing;
    }

    /**
     * @brief draw shape on the canvas
     * @param canvas
     */
    virtual void draw(cv::Mat &canvas) = 0;

    /// helper method for non compund shapes to draw their members
    void drawHelper(cv::Mat &canvas, Shape *other);

    virtual const std::string &getStatusMsg() const;

    void setCanvas(Canvas &value);

    void setDeleted();

    bool isDeleted();

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
    friend void read(const cv::FileNode& node, Canvas& x, const Canvas&);
    friend class Canvas;
    friend class CompoundShape;

    /// called when events happen
    void broadcastEvent(CBEvent event);

    int genId()
    {
        static int idGenerator;
        return ++idGenerator;
    }

    friend void write(cv::FileStorage& fs, const std::string&, const Shape& x);
    friend void read(const cv::FileNode& node, Shape*& x, const Shape* default_value);

    void write(cv::FileStorage& fs) const;
    void read(const cv::FileNode& node);

    std::list<CBType> cbs;
    bool deleted;
};

// These write and read functions must be defined for the serialization in cv::FileStorage to work
void write(cv::FileStorage& fs, const std::string&, const Shape& x);
void read(const cv::FileNode& node, Shape*& x, const Shape* default_value);

// Will write the xml to memory and output it's std::string
std::ostream & operator<<(std::ostream& o, const Shape &shape);

}

#endif // SHAPE_H
