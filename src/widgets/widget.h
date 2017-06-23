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

    /// These are the widget states
    enum State
    {
        ENTER,  ///< got gocus
        LEAVE,  ///< lost focus
        PRESS,  ///< mouse pressed
        RELEASE ///< mouse left
    };

    /// signature of a callback which gets the State
    typedef std::function<void(Widget*, State)> CBWidgetState;

    /// signature of a callback which gets a user selection
    typedef std::function<void(Widget*, int)> CBUserSelection;

    /// constructor
    Widget(Layout &layoutVal, const cv::Point &pos = cv::Point(0,0));

    /// copy constructor
    Widget(const Widget &other);

    /// virtual destructor
    virtual ~Widget();

    /**
     * @brief getType is always implemented by derived to return the same static pointer per widget.
     * @return const char * pointer to string with widget type name
     */
    virtual const char *getType() const = 0;

    /**
     * @brief used to register for notifications on a widget
     * @param cb to invoke on widget state change
     */
    void notifyOnChange(CBWidgetState cb);

    /// get the outline color
    cv::Scalar getOutlineColor() const;

    /// set the outline color
    virtual void setOutlineColor(const cv::Scalar &value);

    /// get the bg color
    cv::Scalar getFillColor() const;

    /// set the bg color
    virtual void setFillColor(const cv::Scalar &value);

    /// is the widget visible
    bool getVisible() const
    {
        return visible;
    }

    /// set the widget visible state
    virtual void setVisible(bool value);

    /// get line thickness to use when drawing
    int getThickness() const;

    /// set line thickness to use when drawing
    virtual void setThickness(int value);

    /// get the line type (LINE_4, LINE_8, LINE_AA)
    int getLineType() const;

    /// set the line type (LINE_4, LINE_8, LINE_AA)
    virtual void setLineType(int value);

    /// get the alpha value used for the widge background [0,255] => [transparent,opaque]
    uchar getAlpha() const;

    /// set the alpha value used for the widge background [0,255] => [transparent,opaque]
    void setAlpha(uchar value);

    /**
     * @brief The Anchor enum
     * Used for both aligment in the Layout we belong to and internal widget alignments
     * @see getLayoutAnchor(), setLayoutAnchor(), getFlowAnchor(), setFlowAnchor()
     */
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

    /**
     * @brief getLayoutAnchor returns the anchor for using in the Layout we're in
     * @return the Anchor related to the Layout we're in
     * @see VerticalLayout, HorizontalLayout
     */
    Anchor getLayoutAnchor() const
    {
        return layoutAnchor;
    }

    /**
     * @brief setLayoutAnchor sets the anchor for using in the Layout we're in
     * @param value is used to set the layoutAnchor
     * @see VerticalLayout, HorizontalLayout
     */
    void setLayoutAnchor(const Anchor &value);

    /**
     * @brief getFlowAnchor affects internal widget alignment and direction of growth
     * @return the Anchor of our own flow
     * @see VerticalLayout, HorizontalLayout, Text
     */
    Anchor getFlowAnchor() const
    {
        return flowAnchor;
    }

    /**
     * @brief setFlowAnchor affects internal widget alignment and direction of growth
     * @param value is used to set the flowAnchor
     * @see VerticalLayout, HorizontalLayout, Text
     */
    void setFlowAnchor(const Anchor &value);

    /// Widgets have a unique id per instance
    int getId()
    {
        return id;
    }

    /**
     * @brief getStatusMsg
     * @return a message to display during mouse hover
     * @see Canvas::enableStatusMsg
     */
    virtual const std::string &getStatusMsg() const;

    /**
     * @brief setStatusMsg
     * @param value a message to display during mouse hover
     * @see Canvas::enableStatusMsg
     */
    void setStatusMsg(const std::string &value);

    /// get widget position in Canvas
    cv::Point getLocation() const;

    /// set widget position in Canvas
    virtual void setLocation(const cv::Point &value);

    /// move the widget
    virtual void translate(const cv::Point &translation);

    /// get if the Layout we're in should stretch us in the X direction
    bool getStretchX() const;

    /// set if the Layout we're in should stretch us in the X direction
    void setStretchX(bool value);

    /// get if the Layout we're in should stretch us in the Y direction
    bool getStretchY() const;

    /// set if the Layout we're in should stretch us in the Y direction
    void setStretchY(bool value);

    /// get the color to use when a widget is selected
    cv::Scalar getSelectColor() const;

    /// set the color to use when a widget is selected
    virtual void setSelectColor(const cv::Scalar &value);

    /// get rid of the widget
    void rmvFromLayout();

    /// Control the widget BG relieft style
    enum Relief
    {
        FLAT,
        RAISED,
        SUNKEN
    };

protected:

    void mergeMats(cv::Mat &roiSrc, cv::Mat &roiDst);

    Relief getRelief() const;
    void setRelief(const Relief &value);

    const cv::Rect getLayoutBoundaries() const;

    /// widgets like buttons change bg on mouse events
    void setStateChangesBG();

    /// invokes Theme::allocateBG()
    void allocateBG(const cv::Size &size);

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
     * @brief render the widget to dst
     * @param dst is the full size image
     */
    virtual void renderOn(cv::Mat &dst);

    /// dst is the roi of the widget size and not the full image
    virtual void drawFG(cv::Mat &dst) = 0;

    /// helper method which delgates to drawFG for derived
    void callDrawFG(bool preAllocateMat=true);

    /// Actual size the widget is occupying due to Layout manager
    virtual const cv::Rect &getRect() = 0;

    /// Minimal size the widget coould have occupy
    virtual const cv::Rect &getMinimalRect() = 0;


    /* TODO - write/read widgets to file for a designer app
    virtual void writeInternals(cv::FileStorage& fs) const = 0;
    virtual void readInternals(const cv::FileNode& node) = 0;
    */

    /**
     * @brief setDirty
     * Mark us as 'dirty' so before the next draw, our 'update' will be called
     * @return true if marked as dirty for later or false if changes were done on the spot
     */
    bool setDirty();

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
    cv::Scalar selectColor;
    Relief relief;
    bool visible;
    int thickness;
    int lineType;
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

    cv::Scalar outlineColor;
    cv::Scalar fillColor;
    cv::Mat bg;
    cv::Mat fg;
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
