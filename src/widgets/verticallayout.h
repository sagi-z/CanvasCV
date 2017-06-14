#ifndef VERTICALLAYOUT_H
#define VERTICALLAYOUT_H

#include "compoundwidget.h"
#include "layoutbasewidget.h"

namespace canvascv
{

class VerticalLayout : public LayoutBaseWidget
{
public:

    static std::shared_ptr<VerticalLayout> create(Layout &layout,
                                                  const cv::Point &pos = cv::Point(0,0));

    virtual const char *getType() const;

    int getSpacing() const;
    void setSpacing(int value);

    virtual void recalc();
    virtual void addWidget(const std::shared_ptr<Widget> &widget);
    virtual bool rmvWidget(const std::shared_ptr<Widget> &widget);

    void rmvWidget(int i);

    template <typename T=Widget>
    T *at(int index);

    static const char *type;

protected:
    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;

    VerticalLayout(Layout &layoutVal, const cv::Point &pos);

    virtual bool replaceTmpSharedPtr(const std::shared_ptr<Widget> &widget);

    /* TODO - write/read widgets to file for a designer app
    virtual void writeInternals(FileStorage &fs) const;
    virtual void readInternals(const FileNode &node);
    */

private:

    int spacing;
    std::vector<std::shared_ptr<Widget>> vertWidgets;
};

// simplest case is the special case
template<>
Widget *VerticalLayout::at(int index);

template<typename T>
T *VerticalLayout::at(int index)
{
    Widget *pWidget = vertWidgets.at(index).get();
    assert (pWidget->getType() == T::type);
    return static_cast<T*>(pWidget);
}

}

#endif // VERTICALLAYOUT_H
