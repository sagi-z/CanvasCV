#ifndef COMPOUNDWIDGET_H
#define COMPOUNDWIDGET_H

#include "widget.h"
#include "layoutbase.h"

#include <list>
#include <memory>

namespace canvascv
{

/**
 * @brief The CompoundWidget class
 * Utility class to help easy creation of complex widgets and Layout managers.
 */
class CompoundWidget : public Widget, public LayoutBase
{
public:

    /// delegate to internal Widget parts added by derived classes
    virtual void setOutlineColor(const cv::Scalar &value);

    /// delegate to internal Widget parts added by derived classes
    virtual void setFillColor(const cv::Scalar &value);

    /// delegate to internal Widget parts added by derived classes
    virtual void setSelectColor(const cv::Scalar &value);

    /// delegate to internal Widget parts added by derived classes
    virtual void setThickness(int value);

    /// delegate to internal Widget parts added by derived classes
    virtual void setLineType(int value);

    /// delegate to internal Widget parts added by derived classes
    virtual void setVisible(bool value);

    /// delegate to internal Widget parts added by derived classes
    virtual const std::string &getStatusMsg() const;

    /// delegate to internal Widget parts added by derived classes
    virtual void translate(const cv::Point &translation);

protected:
    // force inheritance
    CompoundWidget(const cv::Point &pos);
    virtual ~CompoundWidget() {}

    /// delegate to internal Widget parts added by derived classes
    virtual void recalc() final;
    void recalcRect();

    /**
     * @brief recalcCompound
     * Your BG size recalculation/allocation and FG drawing is done here.
     * It is done semi automatically. Is you invoke setters in this method
     * on your internal widgets, then make sure to update them and/or their
     * layout
     */
    virtual void recalcCompound() = 0;

    virtual void addWidget(const std::shared_ptr<Widget> &widget);
    virtual std::shared_ptr<Widget> rmvWidget(const std::shared_ptr<Widget> &widget);

    virtual const cv::Rect getBoundaries() const;

    virtual void renderOn(cv::Mat &dst);
    virtual void drawFG(cv::Mat &dst);
    virtual bool isAtPos(const cv::Point &pos);

    virtual const cv::Rect &getRect();
    virtual const cv::Rect &getMinimalRect();

    virtual void reloadPointers(std::list<Widget*>::const_iterator &)
    {}

    virtual std::shared_ptr<Widget> rmvWidget(Widget* widget);

    /* TODO - write/read widgets to file for a designer app
    virtual void writeInternals(cv::FileStorage &fs) const;
    virtual void readInternals(const cv::FileNode &node);
    */

    bool fillBG;
    cv::Rect rect;

    std::list<std::shared_ptr<Widget>> widgets;
private:
    virtual bool setDirtyLayout();

    virtual void broadcastChange(State status);
    virtual void layoutResized(const cv::Rect &boundaries);

    std::shared_ptr<Widget> active;
    int recalcCalled;
};

}

#endif // COMPOUNDWIDGET_H
