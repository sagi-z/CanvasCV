#ifndef COMPOUNDWIDGET_H
#define COMPOUNDWIDGET_H

#include "widget.h"
#include "widgetfactory.h"
#include "colors.h"

#include <list>
#include <memory>

namespace canvascv
{

/**
 * @brief The CompoundWidget class
 * Utility class to help easy creation of complex widgets and Layout managers.
 */
class CompoundWidget : public Widget
{
public:

    /// delegate to internal Widget parts added by derived classes
    virtual void setOutlineColor(const cv::Scalar &value);

    /// delegate to internal Widget parts added by derived classes
    virtual void setFillColor(const cv::Scalar &value);

    /// delegate to internal Widget parts added by derived classes
    virtual void setSelectColor(const Scalar &value);

    /// delegate to internal Widget parts added by derived classes
    virtual void setThickness(int value);

    /// delegate to internal Widget parts added by derived classes
    virtual void setLineType(int value);

    /// delegate to internal Widget parts added by derived classes
    virtual void setAlpha(double value);

    /// delegate to internal Widget parts added by derived classes
    virtual void setFillBG(bool value);

    /// delegate to internal Widget parts added by derived classes
    virtual void setLocked(bool value);

    /// delegate to internal Widget parts added by derived classes
    virtual void setVisible(bool value);

    /// delegate to internal Widget parts added by derived classes
    virtual const std::string &getStatusMsg() const;

    /// delegate to internal Widget parts added by derived classes
    virtual void recalc();

    /// delegate to internal Widget parts added by derived classes
    virtual void translate(const cv::Point &translation);

protected:
    // force inheritance
    CompoundWidget(Layout &layoutVal, const cv::Point &pos);
    virtual ~CompoundWidget() {}

    virtual void draw(cv::Mat &dst);
    virtual bool isAtPos(const Point &pos);

    virtual const cv::Rect &getRect();
    virtual const cv::Rect &getMinimalRect();

    virtual void reloadPointers(std::list<Widget*>::const_iterator &)
    {}

    void addWidget(const std::shared_ptr<Widget> &widget);
    bool rmvWidget(const std::shared_ptr<Widget> &widget);
    virtual bool rmvWidget(Widget* widget);

    virtual void update();

    bool replaceTmpSharedPtr(const std::shared_ptr<Widget> &widget);

    /* TODO - write/read widgets to file for a designer app
    virtual void writeInternals(cv::FileStorage &fs) const;
    virtual void readInternals(const cv::FileNode &node);
    */

    cv::Rect rect;

private:

    virtual void broadcastChange(State status);
    virtual void layoutResized(const cv::Rect &boundaries);

    std::shared_ptr<Widget> active;
    std::list<std::shared_ptr<Widget>> widgets;

};

}

#endif // COMPOUNDWIDGET_H
