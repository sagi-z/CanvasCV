#ifndef HFRAME_H
#define HFRAME_H

#include "horizontallayout.h"

namespace canvascv
{

/**
 * @brief The HFrame class
 * 
 * A frame with a background of a selected relief type. This is also
 * a Layout for other Widgets and it lays them horizontally.
 * @see HorizontalLayout
 */
class HFrame : public HorizontalLayout
{
public:

    /**
     * @brief create a HFrame widget
     * 
     * @param layout widgets are placed in layouts Canvas/VFrame/HFrame/...
     * @param pos location in the Layout (Layouts can ignore that)
     * @return a smart pointer copy of the object kept in the Layout
     */
    static std::shared_ptr<HFrame> create(Layout &layout,
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

    HFrame(const cv::Point &pos);
};

}
#endif // HFRAME_H
