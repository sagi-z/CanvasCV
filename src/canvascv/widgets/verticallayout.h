#ifndef VERTICALLAYOUT_H
#define VERTICALLAYOUT_H

#include "autolayout.h"

namespace canvascv
{

/**
 * @brief The VerticalLayout class
 * 
 * A Layout implementation which lays it's internal widgets vertically
 * with a predefined spacing between widgets.
 * - Vertical layout is automatic according to widget size and spacing.
 *  -# This layout can expand up (it's 'flowAnchor' is BOTTOM), or down (it's 'flowAnchor' is TOP)
 * - Horizontal layout per laid widget is determined according to the laid widget 'layoutAnchor':
 *  -# LEFT will align the widget to the left (unless setStretchX() is true in the laid widget)
 *  -# CENTER will align the widget to the center (unless setStretchX() is true in the laid widget)
 *  -# RIGHT will align the widget to the right (unless setStretchX() is true in the laid widget)
 * \verbatim
    +----------------------+
    |       PADDING        |
    | P +--------+       P |
    | A | Widget |       A |  layoutAnchor = LEFT
    | D +--------+       D |
    | D     SPACING      D |
    | I   +----------+   I |
    | N   |  Widget  |   N |  layoutAnchor = CENTER
    | G   +----------+   G |
    |       SPACING        |
    |        +---------+   |
    | P      | Widget  | P |  layoutAnchor = RIGHT
    | A      +---------+ A |
    | D     SPACING      D |
    | D +--------------+ D |
    | I |   Widget     | I |  WHATEVER (widest sets the standard for other)
    | N +--------------+ N |
    | G     PADDING      G |
    +----------------------+
 \endverbatim
 */
class VerticalLayout : public AutoLayout
{
public:

    /**
     * @brief create a VerticalLayout widget
     * 
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

    virtual const char *getType() const;

    static const char *type;

protected:
    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;

    VerticalLayout(const cv::Point &pos);

    virtual void recalcCompound();

    /* TODO - write/read widgets to file for a designer app
    virtual void writeInternals(FileStorage &fs) const;
    virtual void readInternals(const FileNode &node);
    */

private:

    int spacing;
};

}

#endif // VERTICALLAYOUT_H
