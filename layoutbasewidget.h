#ifndef LAYOUTBASEWIDGET_H
#define LAYOUTBASEWIDGET_H

#include "layoutbase.h"
#include "compoundwidget.h"

namespace canvascv
{

class LayoutBaseWidget : public LayoutBase, public CompoundWidget
{
public:

    virtual void draw(Mat &dst);

    virtual const Rect getBoundaries() const;

    bool getDrawFrame() const;
    void setDrawFrame(bool value);

    int getPadding() const;
    void setPadding(int value);

protected:
    LayoutBaseWidget(const cv::Point& pos);

    virtual bool isDuringUpdate() const;
    virtual void recalc();

    int padding;

private:
    virtual void setDirtyLayout();

    bool drawFrame;
    cv::Mat colorRect;
};

}
#endif // LAYOUTBASEWIDGET_H
