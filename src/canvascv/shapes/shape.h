#ifndef SHAPE_H
#define SHAPE_H

#include "canvascv/colors.h"
#include "canvascv/consts.h"

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
    Shape();

    /// copy constructor
    Shape(const Shape &other);

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
    cv::Scalar getOutlineColor() const;

    /// set the outline color
    virtual void setOutlineColor(const cv::Scalar &value);

    /// get the fill color (fill color is not very useful for shapes right now)
    cv::Scalar getFillColor() const;

    /// set the fill color (fill color is not very useful for shapes right now)
    virtual void setFillColor(const cv::Scalar &value);

    /// is the shape locked (can't be moved/edited)
    bool getLocked() const;

    /// set the shape lock state (can/can't be moved/edited)
    virtual void setLocked(bool value);

    /// is the shape visible
    bool getVisible() const;

    /// set the shape visible state
    virtual void setVisible(bool value);

    /**
     * @brief getType is always implemented by derived to return the same static pointer per shape.
     * @return const char * pointer to string with shape type name
     */
    virtual const char *getType() const = 0;

    /// get line thickness to use when drawing
    int getThickness() const;

    /// set line thickness to use when drawing
    virtual void setThickness(int value);

    /// get the line type (LINE_4, LINE_8, LINE_AA)
    int getLineType() const;

    /// set the line type (LINE_4, LINE_8, LINE_AA)
    virtual void setLineType(int value);

    /// returns true if shape is at pos, false otherwise
    virtual bool isAtPos(const cv::Point &pos) = 0;

    virtual void translate(const cv::Point &offset) = 0;

    bool isReady() const;

    /// return a unique id for this shape
    int getId();

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
    virtual std::shared_ptr<Shape> getShape(int id);

    /**
     * @brief keyPressed will be called by Canvas for active shapes
     * @param key was pressed. You must set it to -1 if you consumed it.
     * @return true if we want to stay in focus and false otherwise
     */
    virtual bool keyPressed(int &key);

    /**
     * @brief lostFocus is called by Canvas if we're in it and just became non-active
     */
    virtual void lostFocus();

    /**
     * @brief mouseReleased
     * @param pos
     * @return true for keep in focus, false for leave focus
     */
    virtual bool mouseReleased(const cv::Point &pos) = 0;

    bool isEditing();

    /**
     * @brief draw shape on the canvas
     * @param canvas
     */
    virtual void draw(cv::Mat &canvas) = 0;

    /// helper method for non compund shapes to draw their members
    void drawHelper(cv::Mat &canvas, Shape *other);

    const std::string &getStatusMsg() const;
    virtual const std::string &getCreateStatusMsg() const = 0;
    virtual const std::string &getEditStatusMsg() const = 0;

    void setCanvas(Canvas &value);

    void setDeleted();

    bool isDeleted();

    void setReady();

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

    int genId();

    friend void write(cv::FileStorage& fs, const std::string&, const Shape& x);
    friend void read(const cv::FileNode& node, Shape*& x, const Shape* default_value);

    void write(cv::FileStorage& fs) const;
    void read(const cv::FileNode& node);

    std::list<CBType> cbs;
    bool deleted;
    bool ready;
};

// These write and read functions must be defined for the serialization in cv::FileStorage to work
void write(cv::FileStorage& fs, const std::string&, const Shape& x);
void read(const cv::FileNode& node, Shape*& x, const Shape* default_value);

// Will write the xml to memory and output it's std::string
std::ostream & operator<<(std::ostream& o, const Shape &shape);

}

/** @example example_shapes.cpp
 * This is an example of how to create shapes by mouse.
 */

#endif // SHAPE_H
