#ifndef LAYOUTBASEWIDGET_H
#define LAYOUTBASEWIDGET_H

#include "layoutbase.h"
#include "compoundwidget.h"

namespace canvascv
{

class LayoutBaseWidget : public LayoutBase, public CompoundWidget
{
public:

    int getPadding() const;
    void setPadding(int value);

protected:

    virtual const Rect getBoundaries() const;

    bool getDrawFrame() const;
    void setDrawFrame(bool value);

    LayoutBaseWidget(Layout &layoutVal, const cv::Point& pos);

    virtual bool isDuringUpdate() const;
    virtual void recalc();

    int padding;

private:
    virtual void setDirtyLayout();
};

}
#endif // LAYOUTBASEWIDGET_H
