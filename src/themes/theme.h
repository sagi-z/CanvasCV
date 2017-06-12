#ifndef THEME_H
#define THEME_H

#include <opencv2/core.hpp>

namespace canvascv
{

class Widget;
class Shape;

class Theme
{
public:
    /**
     * @brief allocateBG creates the rect background for widgets
     * @param dst is a Mat dedicated to hold the background
     * @param size will be the size of dst when done
     * @param color is the requested color, which the theme is allowed to ignore
     * @param type is the color depth
     */
    virtual void allocateBG(cv::Mat &dst, const cv::Size &size, const cv::Scalar &color, int type = CV_8UC3) = 0;

    /**
     * @brief drawBG draws a rect background previouslt prepared by allocateBG()
     * @param dst is the image on which this is drawn
     * @param rect it the roi inside the dst which will be drawn
     * @param bg was previously allocated by allocateBG()
     * @param alpha in the range of (0, 1) or assumed to be 1 (opaque)
     * @param fillBG is true to draw the full bg or false to draw only the sorrounding rect
     * @param bgOutline will be used when fillBG is false, but the theme is allowed to ignore it
     */
    virtual void drawBG(cv::Mat &dst, const cv::Rect &rect, const cv::Mat &bg,
                        double alpha, bool fillBG, cv::Scalar bgOutline) = 0;

    /**
     * @brief applyStyle is called for every new widget created by the WidgetFactory
     * @param widget will have setters invoked according to the theme
     */
    virtual void applyStyle(Widget *widget) = 0;

    /**
     * @brief applyStyle is called for every new Shape created by the ShapeFactory
     * @param shape will have setters invoked according to the theme
     */
    virtual void applyStyle(Shape *shape) = 0;

    /**
     * @brief applyStateStyle changes bg to match Widget::State
     * @param bg was previously allocated with allocateBG
     * @param state is Widget::State
     */
    virtual void applyStateStyle(cv::Mat &bg, int state) = 0;
};

}

#endif // THEME_H
