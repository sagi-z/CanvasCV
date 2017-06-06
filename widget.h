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

class Layout;

/**
 * @brief The Widget class
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

    // Used both for internal widget alignments and in Layout managers
    enum Anchor
    {
        TOP    = 0b00000001, // unusable alone
        BOTTOM = 0b00000010, // unusable alone
        LEFT   = 0b00000100, // unusable alone
        RIGHT  = 0b00001000, // unusable alone

        // Any of these is a valid anchor
        CENTER = 0b00010000,
        TOP_LEFT      = TOP | LEFT,
        TOP_RIGHT     = TOP | RIGHT,
        BOTTOM_LEFT   = BOTTOM | LEFT,
        BOTTOM_RIGHT  = BOTTOM | RIGHT,
        CENTER_TOP    = CENTER | TOP,
        CENTER_BOTTOM = CENTER | BOTTOM,
        CENTER_LEFT   = CENTER | LEFT,
        CENTER_RIGHT  = CENTER | RIGHT
    };

    typedef std::function<void(Widget*, State)> CBType;

    Widget(const cv::Point &pos = cv::Point(0,0));

    Widget(const Widget &other);

    virtual ~Widget();

    virtual const char *getType() const = 0;

    void notifyOnChange(CBType cb);

    cv::Scalar getOutlineColor() const;

    virtual void setOutlineColor(const cv::Scalar &value);

    cv::Scalar getFillColor() const;

    virtual void setFillColor(const cv::Scalar &value);

    bool getLocked() const
    {
        return locked;
    }

    virtual void setLocked(bool value);

    bool getVisible() const
    {
        return visible;
    }

    virtual void setVisible(bool value);

    int getThickness() const;

    virtual void setThickness(int value);

    int getLineType() const;

    virtual void setLineType(int value);

    virtual Anchor getAnchor() const;

    virtual void setAnchor(const Anchor &value);

    int getId()
    {
        return id;
    }

    virtual const std::string &getStatusMsg() const;
    void setStatusMsg(const std::string &value);

    cv::Point getLocation() const;
    virtual void setLocation(const cv::Point &value);

    virtual void translate(const cv::Point &translation);

protected:

    /// update self so next call to 'draw' will display correctly
    virtual void recalc() = 0;

    /**
     * @brief draw the widget
     * @param dst
     */
    virtual void draw(cv::Mat &dst) = 0;

    /// Actual size the widget is occupying due to Layout manager
    virtual const cv::Rect &getRect() = 0;

    /// Minimal size the widget coould have occupy
    virtual const cv::Rect &getMinimalRect() = 0;

    virtual void writeInternals(cv::FileStorage& fs) const = 0;
    virtual void readInternals(const cv::FileNode& node) = 0;

    /// Mark us as 'dirty' so before the next draw, our 'update' will be called
    void setDirty();

    /// Removes 'dirty' state and invokes the user defined 'recalc'
    void update();

    /**
     * @brief isAtPos
     * @param pos
     * @return true or false if at location
     */
    virtual bool isAtPos(const cv::Point &pos);

    State getState() const;

    /// Used by Layout managers
    void stretchWidth(int width);
    void stretchHeight(int height);


    int id;
    cv::Point location;
    cv::Scalar outlineColor;
    cv::Scalar fillColor;
    bool locked;
    bool visible;
    int thickness;
    int lineType;
    int forcedWidth;
    int forcedHeight;
    Anchor anchor;
    std::string statusMsg;
    Layout *layout;

private:
    friend class Canvas;
    friend class CompoundWidget;
    friend class LayoutBase;
    friend class VerticalLayout;

    /**
     * @brief mousePressed
     * Called by layout when mouse pressed on us
     */
    virtual void mousePressed() = 0;

    /**
     * @brief mouseReleased
     * Called by layout when mouse released from us
     */
    virtual void mouseReleased() = 0;

    /**
     * @brief mouseEnter
     * Called by layout when mouse entered
     */
    virtual void mouseEnter() = 0;

    /**
     * @brief mouseEnter
     * Called by layout when mouse left
     */
    virtual void mouseLeave() = 0;

    int genId();

    bool getIsDirty() const;

    void setLayout(Layout &value);
    Layout* getLayout();

    virtual void layoutResized(const cv::Rect &boundaries);

    /// called by the canvas when the widget changes state
    virtual void broadcastChange(State status);

    friend void write(cv::FileStorage& fs, const std::string&, const Widget& x);
    friend void read(const cv::FileNode& node, Widget*& x, const Widget* default_value);

    void write(cv::FileStorage& fs) const;
    void read(const cv::FileNode& node);

    State state;
    bool isDirty;
    std::list<CBType> changeNotifs;
};

// These write and read functions must be defined for the serialization in cv::FileStorage to work
void write(cv::FileStorage& fs, const std::string&, const Widget& x);
void read(const cv::FileNode& node, Widget*& x, const Widget* default_value);

// Will write the xml to memory and output it's std::string
std::ostream & operator<<(std::ostream& o, const Widget &shape);

}

#endif // WIDGET_H
