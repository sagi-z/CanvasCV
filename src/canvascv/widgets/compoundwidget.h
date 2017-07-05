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
 * 
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

    /// delegate to active widget or to our derived class if none active
    virtual const std::string &getStatusMsg() const;

    /// delegate to internal Widget parts added by derived classes
    virtual void translate(const cv::Point &translation);

    /**
     * @brief doForAll
     * 
     * invoke the cb for all included widgets.
     * This is usually not necessary since the above methods delegate to both
     * contained widgets and self.
     * @param cb is what to execute on widgets
     * @param recurseLevel is how deep to go when updating kids:
     *  - -1 means all decendents at all levels
     *  -  0 means only current level (doOnSelf needs to be also 'true')
     *  -  n>0 means up to kids at level n (1 is children, 2 is also grandchildren, etc.)
     * @param doOnSelf is if to also invoke the cb on the first parent
     */
    void doForAll(CBWidget cb, int recurseLevel, bool doOnSelf);

    virtual const cv::Rect &getRect();
protected:
    // force inheritance
    CompoundWidget(const cv::Point &pos);
    virtual ~CompoundWidget() {}

    virtual bool isCompoundWidget() const;

    /// delegate to internal Widget parts added by derived classes
    virtual void recalc() final;
    void recalcRect(int padding);

    /**
     * @brief recalcCompound
     * 
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
    cv::Rect minimalRect;

    std::list<std::shared_ptr<Widget>> widgets;
private:
    virtual bool setDirtyLayout();

    virtual void broadcastChange(State status);

    void recalcMinimalRect();

    std::shared_ptr<Widget> active;
};

}

#endif // COMPOUNDWIDGET_H
