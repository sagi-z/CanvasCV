#ifndef AUTOLAYOUT_H
#define AUTOLAYOUT_H

#include "layoutbase.h"
#include "compoundwidget.h"

namespace canvascv
{

/**
 * @brief The AutoLayout class
 * 
 * Base class for all Layout managers which are also widgets (not the Canvas class)
 */
class AutoLayout : public CompoundWidget
{
public:

    /**
     * @brief getPadding get number of pixels to pad from Layout rect during layout
     * 
     * \verbatim
        +----------------------+
        |       PADDING        |
        |   +--------------+   |
        | P |   Widget     | P |
        | A +--------------+ A |
        | D     SPACING      D |
        | D +--------------+ D |
        | I |   Widget     | I |
        | N +--------------+ N |
        | G     PADDING      G |
        +----------------------+
     \endverbatim
     * @return padding in pixels
     */
    int getPadding() const;

    /**
     * @brief setPadding set number of pixels to pad from Layout rect during layout
     * 
     * \verbatim
        +----------------------+
        |       PADDING        |
        |   +--------------+   |
        | P |   Widget     | P |
        | A +--------------+ A |
        | D     SPACING      D |
        | D +--------------+ D |
        | I |   Widget     | I |
        | N +--------------+ N |
        | G     PADDING      G |
        +----------------------+
     \endverbatim
     * @return
     */
    void setPadding(int value);

    virtual void addWidget(const std::shared_ptr<Widget> &widget);
    virtual std::shared_ptr<Widget> rmvWidget(const std::shared_ptr<Widget> &widget);

    /// remove a widget at index 'i' (silently ignores 'i' too big)
    void rmvWidget(int i);

    /// return a widget at index 'i' (or 0 if type is wrong ot 'i' is too big)
    template <typename T=Widget>
    T *at(int index);

    /// returns the number of widgets in the layout
    size_t size() const;

    /// get if the layout will wrap widgets past forcedWidth / forcedHeight
    bool getWrap() const;

    /// set if the layout will wrap widgets past forcedWidth / forcedHeight
    void setWrap(bool value);

protected:

    AutoLayout(const cv::Point& pos);

    void recalcAndAllocate();
    virtual const cv::Rect getBoundaries() const;

    int padding;
    bool wrap;
};

// simplest case is the special case
template<>
Widget *AutoLayout::at(int index);

template<typename T>
T *AutoLayout::at(int index)
{
    Widget *pWidget = nullptr;
    if (index < widgets.size())
    {
        auto i = widgets.begin();
        std::advance(i, index);
        pWidget = i->get();
        assert (pWidget->getType() == T::type);
    }
    return static_cast<T*>(pWidget);
}

}
#endif // AUTOLAYOUT_H
