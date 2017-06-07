#ifndef LAYOUTBASEWIDGET_H
#define LAYOUTBASEWIDGET_H

#include "layoutbase.h"
#include "compoundwidget.h"

namespace canvascv
{

class LayoutBaseWidget : public LayoutBase, public CompoundWidget
{
public:

    virtual const Rect getBoundaries() const;

protected:
    LayoutBaseWidget(const cv::Point& pos);

    virtual bool isDuringUpdate() const;

private:
    virtual void setDirtyLayout();
};

}
#endif // LAYOUTBASEWIDGET_H
