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

    // Used both for internal widget flow and in Layout managers
    // Widget flow:
    // ===========
    // * TOP (default) - means the widget 'location' is the
    //  top-left cv::Point of the widget cv::Rect. The contets are
    //  filled downwards.
    // * BOTTOM - means the widget 'location' is the bottom-left
    //  cv::Point of the widget cv::Rect. The contets are filled
    //  upwards.
    // * For widgets that are also layout managers, you can control
    //  the direction in which they progress by also anchoring them to
    //  the opposite side -
    //   - vertical layout can progress either up or down, so you
    //    can anchor it also to TOP (default) / BOTTOM.
    //   - horizontal layout can progress either left or right, so you
    //    can anchor it also to LEFT (default) / RIGHT.
    //
    // Layout alignment:
    // ================
    // * For a vertical layout - if the stretchX is false, you can
    //  align (anchor) the widget horizontally to the LEFT (default) / CENTER / RIGHT.
    //   The layout manager will handle vetical layout according to its own
    //  widget flow - anchored to the TOP (default) or BOTTOM.
    // * For a horizontal layout - if the stretchY is false, you can
    //  align (anchor) the widget vertically to the TOP (default) / CENTER / BOTTOM.
    //   The layout manager will handle horizontal layout according to its own
    //  widget flow - anchored to the LEFT (default) or RIGHT.
    enum Relief
    {
        FLAT,
        RAISED,
        SUNKEN
    };

    enum Anchor
    {
        TOP    = 0b00000001,
        BOTTOM = 0b00000010,
        LEFT   = 0b00000100,
        RIGHT  = 0b00001000,
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


    enum State
    {
        ENTER,
        LEAVE,
        PRESS,
        RELEASE
    };

    typedef std::function<void(Widget*, State)> CBWidgetState;

    typedef std::function<void(int)> CBUserSelection;

    Widget(Layout &layoutVal, const cv::Point &pos = cv::Point(0,0));

    Widget(const Widget &other);

    virtual ~Widget();

    virtual const char *getType() const = 0;

    void notifyOnChange(CBWidgetState cb);

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

    double getAlpha() const;
    virtual void setAlpha(double value);

    std::string getMsg() const;
    void setMsg(const std::string &value);


    Anchor getLayoutAnchor() const
    {
        return layoutAnchor;
    }

    Anchor getFlowAnchor() const
    {
        return flowAnchor;
    }

    void setLayoutAnchor(const Anchor &value);

    void setFlowAnchor(const Anchor &value);

    int getId()
    {
        return id;
    }

    virtual const std::string &getStatusMsg() const;
    void setStatusMsg(const std::string &value);

    cv::Point getLocation() const;
    virtual void setLocation(const cv::Point &value);

    virtual void translate(const cv::Point &translation);

    bool getStretchX() const;
    void setStretchX(bool value);

    bool getStretchY() const;
    void setStretchY(bool value);

    cv::Scalar getSelectColor() const;
    virtual void setSelectColor(const cv::Scalar &value);

    void rmvFromLayout();

protected:

    bool getFillBG() const;
    virtual void setFillBG(bool value);

    Relief getRelief() const;
    void setRelief(const Relief &value);

    const cv::Rect getLayoutBoundaries() const;

    /// widgets like buttons change bg on mouse events
    void setStateChangesBG();

    /// invokes Theme::allocateBG()
    void allocateBG(const cv::Size &size, int type = CV_8UC3);

    /// invokes Theme::drawBG()
    void drawBG(cv::Mat &dst, const cv::Rect &rect);

    /// update self so next call to 'draw' will display correctly
    virtual void recalc() = 0;

    /// delegate to Theme
    void flatWidget();

    /// delegate to Theme
    void raisedWidget();

    /// delegate to Theme
    void sunkenWidget();

    /// delegate to Theme
    void selectedWidget();

    /**
     * @brief draw the widget
     * @param dst
     */
    virtual void draw(cv::Mat &dst);

    /// dst is the roi of the widget and not full image
    virtual void drawFG(cv::Mat &dst);

    /// Actual size the widget is occupying due to Layout manager
    virtual const cv::Rect &getRect() = 0;

    /// Minimal size the widget coould have occupy
    virtual const cv::Rect &getMinimalRect() = 0;


    /* TODO - write/read widgets to file for a designer app
    virtual void writeInternals(cv::FileStorage& fs) const = 0;
    virtual void readInternals(const cv::FileNode& node) = 0;
    */

    /// Mark us as 'dirty' so before the next draw, our 'update' will be called
    void setDirty();

    bool getIsDirty() const;

    /// Removes 'dirty' state and invokes the user defined 'recalc'
    virtual void update();

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
    cv::Scalar selectColor;
    Relief relief;
    bool locked;
    bool visible;
    int thickness;
    int lineType;
    double alpha;
    bool fillBG;
    int forcedWidth;
    int forcedHeight;
    Anchor layoutAnchor;
    Anchor flowAnchor;
    bool stretchX;
    bool stretchY;
    std::string statusMsg;
    Layout *layout;

private:
    friend class Canvas;
    friend class CompoundWidget;

    // All the layout implementations are friends
    friend class LayoutBase;
    friend class AutoLayout;
    friend class VerticalLayout;
    friend class HorizontalLayout;

    /**
     * @brief mousePressed
     * Called by layout when mouse pressed on us
     */
    virtual void mousePressed();

    /**
     * @brief mouseReleased
     * Called by layout when mouse released from us
     */
    virtual void mouseReleased();

    /**
     * @brief mouseEnter
     * Called by layout when mouse entered
     */
    virtual void mouseEnter();

    /**
     * @brief mouseEnter
     * Called by layout when mouse left
     */
    virtual void mouseLeave();

    int genId();

    void setLayout(Layout &value);
    Layout* getLayout();

    virtual void layoutResized(const cv::Rect &boundaries);

    /// called by the canvas when the widget changes state
    virtual void broadcastChange(State status);

    /* TODO - write/read widgets to file for a designer app
    friend void write(cv::FileStorage& fs, const std::string&, const Widget& x);
    friend void read(const cv::FileNode& node, Widget*& x, const Widget* default_value);

    void write(cv::FileStorage& fs) const;
    void read(const cv::FileNode& node);
    */

    cv::Mat bg;
    State state;
    bool isDirty;
    bool delayedUpdate;
    std::list<CBWidgetState> changeNotifs;
};

/* TODO - write/read widgets to file for a designer app
// These write and read functions must be defined for the serialization in cv::FileStorage to work
void write(cv::FileStorage& fs, const std::string&, const Widget& x);
void read(const cv::FileNode& node, Widget*& x, const Widget* default_value);

// Will write the xml to memory and output it's std::string
std::ostream & operator<<(std::ostream& o, const Widget &widget);
*/

}

#endif // WIDGET_H
