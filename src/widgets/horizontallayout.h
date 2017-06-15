#ifndef HORIZONTALLAYOUT_H
#define HORIZONTALLAYOUT_H

#include "compoundwidget.h"
#include "layoutbasewidget.h"

namespace canvascv
{

/**
 * @brief The HorizontalLayout class
 * A Layout implementation which lays it's internal widgets horizontally
 * with a predefined spacing between widgets.
 * @note
 * - Horizontal layout is automatic according to widget size and spacing.
 *  -# This layout can expand left (it's 'flowAnchor' is RIGHT), or left (it's 'flowAnchor' is RIGHT)
 * - Vertical layout per layed widget is determined according to the layed widget 'layoutAnchor':
 *  -# TOP will align the widget to the top (unless setStretchY() is true in the layed widget)
 *  -# CENTER will align the widget to the center (unless setStretchY() is true in the layed widget)
 *  -# BOTTOM will align the widget to the bottom (unless setStretchY() is true in the layed widget)
 */
class HorizontalLayout : public LayoutBaseWidget
{
public:

    /**
     * @brief create a HorizontalLayout widget
     * @param layout widgets are placed in layouts Canvas/VFrame/HFrame/...
     * @param pos location in the Layout (Layouts can ignore that)
     * @return a smart pointer copy of the object kept in the Layout
     */
    static std::shared_ptr<HorizontalLayout> create(Layout &layout,
                                                  const cv::Point &pos = cv::Point(0,0));

    virtual const char *getType() const;

    /// returns pixel spacing between items
    int getSpacing() const;

    /// sets pixel spacing between items
    void setSpacing(int value);

    virtual void addWidget(const std::shared_ptr<Widget> &widget);
    virtual bool rmvWidget(const std::shared_ptr<Widget> &widget);

    /// remove a widget at index 'i' (silently ignores 'i' too big)
    void rmvWidget(int i);

    /// return a widget at index 'i' (or 0 if type is wrong ot 'i' is too big)
    template <typename T=Widget>
    T *at(int index);

    static const char *type;

protected:
    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;

    HorizontalLayout(Layout &layoutVal, const cv::Point &pos);

    virtual void recalc();

    virtual bool replaceTmpSharedPtr(const std::shared_ptr<Widget> &widget);

    /* TODO - write/read widgets to file for a designer app
    virtual void writeInternals(FileStorage &fs) const;
    virtual void readInternals(const FileNode &node);
    */

private:

    int spacing;
    std::vector<std::shared_ptr<Widget>> horzWidgets;
};

// simplest case is the special case
template<>
Widget *HorizontalLayout::at(int index);

template<typename T>
T *HorizontalLayout::at(int index)
{
    Widget *pWidget = horzWidgets.at(index).get();
    assert (pWidget->getType() == T::type);
    return static_cast<T*>(pWidget);
}

}

#endif // HORIZONTALLAYOUT_H
