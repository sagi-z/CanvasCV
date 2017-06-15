#ifndef VERTICALLAYOUT_H
#define VERTICALLAYOUT_H

#include "compoundwidget.h"
#include "layoutbasewidget.h"

namespace canvascv
{

/**
 * @brief The VerticalLayout class
 * A Layout implementation which lays it's internal widgets vertically
 * with a predefined spacing between widgets.
 * - Vertical layout is automatic according to widget size and spacing.
 *  -# This layout can expand up (it's 'flowAnchor' is BOTTOM), or down (it's 'flowAnchor' is TOP)
 * - Horizontal layout per layed widget is determined according to the layed widget 'layoutAnchor':
 *  -# LEFT will align the widget to the left (unless setStretchX() is true in the layed widget)
 *  -# CENTER will align the widget to the center (unless setStretchX() is true in the layed widget)
 *  -# RIGHT will align the widget to the right (unless setStretchX() is true in the layed widget)
 *  +----------------------+
 *  |       PADDING        |
 *  | P +--------+       P |
 *  | A | Widget |       A |  layoutAnchor = LEFT
 *  | D +--------+       D |
 *  | D     SPACING      D |
 *  | I   +----------+   I |
 *  | N   |  Widget  |   N |  layoutAnchor = CENTER
 *  | G   +----------+   G |
 *  |       SPACING        |
 *  |        +---------+   |
 *  | P      | Widget  | P |  layoutAnchor = RIGHT
 *  | A      +---------+ A |
 *  | D     SPACING      D |
 *  | D +--------------+ D |
 *  | I |   Widget     | I |  WHATEVER (widest sets the standard for other)
 *  | N +--------------+ N |
 *  | G     PADDING      G |
 *  +----------------------+
 */
class VerticalLayout : public LayoutBaseWidget
{
public:

    /**
     * @brief create a VerticalLayout widget
     * @param layout widgets are placed in layouts Canvas/VFrame/HFrame/...
     * @param pos location in the Layout (Layouts can ignore that)
     * @return a smart pointer copy of the object kept in the Layout
     */
    static std::shared_ptr<VerticalLayout> create(Layout &layout,
                                                  const cv::Point &pos = cv::Point(0,0));

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

    virtual const char *getType() const;

    static const char *type;

protected:
    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;

    VerticalLayout(Layout &layoutVal, const cv::Point &pos);

    virtual void recalc();

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
