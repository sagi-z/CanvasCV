#ifndef MATWIDGET_H
#define MATWIDGET_H

#include <opencv2/opencv.hpp>
#include <list>
#include "canvascv/colors.h"
#include "widget.h"

namespace canvascv
{

/**
 * @brief The MatWidget class
 * 
 * Displaying a Mat on an OpenCV window, with alpha channel support.
 */
class MatWidget : public Widget
{
public:
    /**
     * @brief create a MatWidget widget
     * 
     * @param layout widgets are placed in layouts Canvas/VFrame/HFrame/...
     * @param mat will be referenced by this widget (not copied)
     * @param pos location in the Layout (Layouts can ignore that)
     */
    static std::shared_ptr<MatWidget> create(Layout &layout,
                                             cv::Mat mat = cv::Mat(),
                                             const cv::Point &pos = cv::Point(0,0));


    /// get the FG Mat which is displayed by the widget
    const cv::Mat &getMat() const;

    /**
     * @brief setMat
     * 
     * set the FG Mat which is displayed by the widget
     * @param value will assigned (not copied) to an internal Mat
     */
    void setMat(const cv::Mat &value);

    virtual const char *getType() const;
    static const char *type;
protected:
    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;

    virtual const cv::Rect &getRect();
    virtual const cv::Rect &getMinimalRect();

    MatWidget(const cv::Point &pos);

    virtual void recalc();

    virtual void drawFG(cv::Mat &dst);

    /* TODO - write/read widgets to file for a designer app
    virtual void writeInternals(cv::FileStorage &fs) const
    {
    }
    virtual void readInternals(const cv::FileNode &node)
    {
    }
    */

private:
    cv::Mat mat;
    cv::Mat displayedMat;
    cv::Rect rect;
    cv::Rect minimalRect;
};

}

/** @example example_matwidget.cpp
 * This is an example of how to use the MatWidget Widget.
 */

#endif // MATWIDGET_H
