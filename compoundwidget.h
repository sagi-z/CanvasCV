#ifndef COMPOUNDWIDGET_H
#define COMPOUNDWIDGET_H

#include "widget.h"
#include "widgetfactory.h"
#include "colors.h"

#include <list>
#include <memory>

namespace canvascv
{

class CompoundWidget : public Widget
{
public:
    virtual void setLocation(const Point &value);
    virtual void setOutlineColor(const cv::Scalar &value);
    virtual void setFillColor(const cv::Scalar &value);
    virtual void setThickness(int value);
    virtual void setLineType(int value);
    virtual void setLocked(bool value);
    virtual void setVisible(bool value);
    virtual void setLayoutAnchor(const Anchor &value);
    virtual void setFlowAnchor(const Anchor &value);
    virtual const std::string &getStatusMsg() const;

    virtual void recalc();

    virtual void translate(const cv::Point &translation);

protected:
    // force inheritance
    CompoundWidget(const cv::Point &pos);
    virtual ~CompoundWidget() {}

    virtual void draw(cv::Mat &dst);
    virtual bool isAtPos(const Point &pos);

    virtual const cv::Rect &getRect();
    virtual const cv::Rect &getMinimalRect();

    virtual void reloadPointers(std::list<Widget*>::const_iterator &)
    {}

    template <class T>
    T* addWidget(const cv::Point &pos);

    void addWidget(const std::shared_ptr<Widget> &widget);
    bool rmvWidget(const std::shared_ptr<Widget> &widget);
    virtual bool rmvWidget(Widget* widget);

    virtual void writeInternals(cv::FileStorage &fs) const;
    virtual void readInternals(const cv::FileNode &node);

    cv::Rect rect;

private:

    virtual void broadcastChange(State status);
    virtual void layoutResized(const cv::Rect &boundaries);
    virtual void mousePressed() final {}
    virtual void mouseReleased() final {}
    virtual void mouseEnter() final {}
    virtual void mouseLeave() final {}

    std::shared_ptr<Widget> active;
    std::list<std::shared_ptr<Widget>> widgets;

};

template <class T> inline
T* CompoundWidget::addWidget(const cv::Point &pos)
{
    T *ret = dynamic_cast<T*>(WidgetFactoryT<T>::newWidget(pos));
    widgets.push_back(std::shared_ptr<Widget>(ret));
    return ret;
}

}

#endif // COMPOUNDWIDGET_H
