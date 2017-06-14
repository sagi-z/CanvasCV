#ifndef HORIZONTALLAYOUT_H
#define HORIZONTALLAYOUT_H

#include "compoundwidget.h"
#include "layoutbasewidget.h"

namespace canvascv
{

class HorizontalLayout : public LayoutBaseWidget
{
public:

    HorizontalLayout(Layout &layoutVal, const cv::Point &pos);

    static std::shared_ptr<HorizontalLayout> create(Layout &layout,
                                                  const cv::Point &pos = cv::Point(0,0));

    virtual const char *getType() const;

    int getSpacing() const;
    void setSpacing(int value);

    virtual void recalc();
    virtual void addWidget(const std::shared_ptr<Widget> &widget);
    virtual bool rmvWidget(const std::shared_ptr<Widget> &widget);

    void rmvWidget(int i);

    Widget *at(int index);

    static const char *type;

protected:

    virtual bool replaceTmpSharedPtr(const std::shared_ptr<Widget> &widget);

    /* TODO - write/read widgets to file for a designer app
    virtual void writeInternals(FileStorage &fs) const;
    virtual void readInternals(const FileNode &node);
    */

private:

    int spacing;
    std::vector<std::shared_ptr<Widget>> horzWidgets;
};

}

#endif // HORIZONTALLAYOUT_H
