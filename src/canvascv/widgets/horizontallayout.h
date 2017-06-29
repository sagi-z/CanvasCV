#ifndef HORIZONTALLAYOUT_H
#define HORIZONTALLAYOUT_H

#include "autolayout.h"

namespace canvascv
{

/**
 * @brief The HorizontalLayout class
 * A Layout implementation which lays it's internal widgets horizontally
 * with a predefined spacing between widgets.
 * - Horizontal layout is automatic according to widget size and spacing.
 *  -# This layout can expand left (it's 'flowAnchor' is RIGHT), or left (it's 'flowAnchor' is RIGHT)
 * - Vertical layout per laid widget is determined according to the laid widget 'layoutAnchor':
 *  -# TOP will align the widget to the top (unless setStretchY() is true in the laid widget)
 *  -# CENTER will align the widget to the center (unless setStretchY() is true in the laid widget)
 *  -# BOTTOM will align the widget to the bottom (unless setStretchY() is true in the laid widget)
 * \verbatim
         WHATEVER
    (highest sets the standard for other)
            ^
            |
    +-------------------------------------------------------------------------------+
    |       PADDING            PADDING            PADDING            PADDING        |
    | P +--------------+ S +--------------+ S                  S                  P |
    | A |              | P |    Widget    | P +--------------+ P                  A |
    | D |    Widget    | A +--------------+ A |    Widget    | A                  D |
    | D |              | C                  C |              | C +--------------+ D |
    | I |              | I                  I +--------------+ I |    Widget    | I |
    | N +--------------+ N                  N                  N +--------------+ N |
    | G     PADDING      G     PADDING      G     PADDING      G     PADDING      G |
    +-------------------------------------------------------------------------------+
                          layoutAnchor=TOP   layoutAnchor=CENTER layoutAnchor=BOTTOM
 \endverbatim
 */
class HorizontalLayout : public AutoLayout
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

    static const char *type;

protected:
    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;

    HorizontalLayout(const cv::Point &pos);

    virtual void recalcCompound();

    /* TODO - write/read widgets to file for a designer app
    virtual void writeInternals(FileStorage &fs) const;
    virtual void readInternals(const FileNode &node);
    */

private:

    int spacing;
};

}

#endif // HORIZONTALLAYOUT_H
