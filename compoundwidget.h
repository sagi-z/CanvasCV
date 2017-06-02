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
    virtual void draw(cv::Mat &canvas);

    virtual void setOutlineColor(const cv::Scalar &value);
    virtual void setFillColor(const cv::Scalar &value);
    virtual void setThickness(int value);
    virtual void setLineType(int value);
    virtual void setLocked(bool value);
    virtual void setVisible(bool value);

    virtual bool isAtPos(const Point &pos);

protected:
    virtual ~CompoundWidget() {} // force inheritance

    virtual void reloadPointers(std::list<Widget*>::const_iterator &)
    {}

    template <class T>
    T* addWidget(cv::Point pos);

    bool rmvWidget(std::shared_ptr<Widget> &widget);

    virtual void writeInternals(cv::FileStorage &fs) const;
    virtual void readInternals(const cv::FileNode &node);

private:

    virtual void broadcastChange(State status);
    virtual void canvasResized(const cv::Size &size);
    virtual void mousePressed() final {}
    virtual void mouseReleased() final {}
    virtual void mouseEnter() final {}
    virtual void mouseLeave() final {}

    std::shared_ptr<Widget> active;
    std::list<std::shared_ptr<Widget>> widgets;
};

template <class T> inline
T* CompoundWidget::addWidget(cv::Point pos)
{
    T *ret = dynamic_cast<T*>(WidgetFactoryT<T>::newWidget(pos));
    widgets.push_back(std::shared_ptr<Widget>(ret));
    return ret;
}

}

#endif // COMPOUNDWIDGET_H
