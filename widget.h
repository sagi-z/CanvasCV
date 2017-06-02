#ifndef WIDGET_H
#define WIDGET_H

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
 * @brief The Widget class
 * Code from outside of this namespace should only use members marked as PUBLIC.
 * INTERNAL api usage might cause instability and compatibility issues.
 */
class Widget
{
public:
    enum State
    {
        ENTER,
        LEAVE,
        PRESS,
        RELEASE
    };

    typedef std::function<void(Widget*, State)> CBType;

    Widget()
        : id(genId()),
          outlineColor(Colors::GREEN),
          fillColor(outlineColor),
          locked(false),
          visible(true),
          thickness(1),
          lineType(cv::LINE_AA),
          canvas(nullptr),
          state(LEAVE)
    {}

    Widget(const Widget &other)
        : id(genId()),
          outlineColor(other.outlineColor),
          fillColor(other.fillColor),
          locked(other.locked),
          visible(other.visible),
          thickness(other.thickness),
          lineType(other.lineType),
          canvas(other.canvas),
          state(LEAVE)
    {}

    virtual ~Widget();

    /**
     * @brief PUBLIC: used to register for notifications on shape seletion by user
     * @param cb to invoke on shape creation
     */
    void notifyOnChange(CBType cb);

    /**
     * @brief INTERNAL: draw shape on the canvas
     * @param canvas
     */
    virtual void draw(cv::Mat &canvas) = 0;

    /**
     * @brief INTERNAL: isAtPos
     * @param pos
     * @return true or false if at location
     */
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

    /**
     * @brief PUBLIC: getType should be implemented by derived.
     * @return c str of type
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

    const std::string &getStatusMsg() const;
    void setStatusMsg(const std::string &value);

protected:
    State getState() const;

    virtual void writeInternals(cv::FileStorage& fs) const = 0;
    virtual void readInternals(const cv::FileNode& node) = 0;

    int id;
    cv::Scalar outlineColor;
    cv::Scalar fillColor;
    bool locked;
    bool visible;
    int thickness;
    int lineType;
    std::string statusMsg;
    Canvas *canvas;

private:
    friend class Canvas;
    friend class CompoundWidget;

    /**
     * @brief setCanvas
     * Called when canvas is creating a shape.
     * Only realy used by specific shapes.
     * @param value
     */
    void setCanvas(Canvas &value);

    virtual void canvasResized(const cv::Size &size) {}

    /// called by the canvas when the shape is (un)selected
    virtual void broadcastChange(State status);

    /**
     * @brief INTERNAL: mousePressed
     * Called by canvas when mouse pressed on us
     */
    virtual void mousePressed() = 0;

    /**
     * @brief INTERNAL: mouseReleased
     * Called by canvas when mouse released from us
     */
    virtual void mouseReleased() = 0;

    /**
     * @brief INTERNAL: mouseEnter
     * Called by canvas when mouse entered
     */
    virtual void mouseEnter() = 0;

    /**
     * @brief INTERNAL: mouseEnter
     * Called by canvas when mouse left
     */
    virtual void mouseLeave() = 0;

    int genId()
    {
        static int idGenerator;
        return ++idGenerator;
    }

    friend void write(cv::FileStorage& fs, const std::string&, const Widget& x);
    friend void read(const cv::FileNode& node, Widget*& x, const Widget* default_value);

    void write(cv::FileStorage& fs) const;
    void read(const cv::FileNode& node);

    State state;
    std::list<CBType> changeNotifs;
};

// These write and read functions must be defined for the serialization in cv::FileStorage to work
void write(cv::FileStorage& fs, const std::string&, const Widget& x);
void read(const cv::FileNode& node, Widget*& x, const Widget* default_value);

// Will write the xml to memory and output it's std::string
std::ostream & operator<<(std::ostream& o, const Widget &shape);

}

#endif // WIDGET_H
