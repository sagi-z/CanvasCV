#ifndef THEME_H
#define THEME_H

#include <opencv2/core.hpp>

namespace canvascv
{

class Widget;
class Shape;

/**
 * @brief The Theme controls appearance of widgets and shapes
 * Themes are added to the ThemeRepository
 */
class Theme
{
public:
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
     * @param bgColor will be used when fillBG is false, but the theme is allowed to ignore it
     */
    virtual void drawBG(cv::Mat &dst, const cv::Rect &rect, const cv::Mat &bg,
                        double alpha, bool fillBG, const cv::Scalar &bgColor) = 0;

    /**
     * @brief flat will cause bg to appear flat
     * @param bg was previously allocated by allocateBG()
     * @param color could be used for drawing, but the theme is allowed to ignore it
     */
    virtual void flat(cv::Mat &bg, const cv::Scalar &color) = 0;

    /**
     * @brief raised will cause bg to appear raised
     * @param bg was previously allocated by allocateBG()
     * @param color could be used for drawing, but the theme is allowed to ignore it
     */
    virtual void raised(cv::Mat &bg, const cv::Scalar &color) = 0;

    /**
     * @brief sunken will cause bg to appear sunken
     * @param bg was previously allocated by allocateBG()
     * @param color could be used for drawing, but the theme is allowed to ignore it
     */
    virtual void sunken(cv::Mat &bg, const cv::Scalar &color) = 0;


    /**
     * @brief selected will cause bg to appear selecte
     * @param bg was previously allocated by allocateBG()
     * @param color could be used for drawing, but the theme is allowed to ignore it
     */
    virtual void selected(cv::Mat &bg, const cv::Scalar &color) = 0;
};

}

#endif // THEME_H
