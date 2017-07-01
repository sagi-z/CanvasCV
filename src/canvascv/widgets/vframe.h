#ifndef VFRAME_H
#define VFRAME_H

#include "verticallayout.h"

namespace canvascv
{

/**
 * @brief The VFrame class
 * 
 * A frame with a background of a selected relief type. This is also
 * a Layout for other Widgets and it lays them vertically.
 * @see VerticalLayout
 */
class VFrame : public VerticalLayout
{
public:

    /**
     * @brief create a VFrame widget
     * 
     * @param layout widgets are placed in layouts Canvas/VFrame/HFrame/...
     * @param pos location in the Layout (Layouts can ignore that)
     * @return a smart pointer copy of the object kept in the Layout
     */
    static std::shared_ptr<VFrame> create(Layout &layout,
                                          const cv::Point &pos = cv::Point(0,0));

    /**
     * @brief setFrameRelief
     * 
     * @param value is one of the supported relief types
     * @see Widget::Relief
     */
    void setFrameRelief(Relief value);

    virtual const char *getType() const;

    static const char *type;

protected:
    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;

    VFrame(const cv::Point &pos);
};

}
#endif // VFRAME_H
