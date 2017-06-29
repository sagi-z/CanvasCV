#ifndef THEME_H
#define THEME_H

#include <opencv2/core.hpp>

namespace canvascv
{

class Widget;
class Shape;

/**
 * @brief The Theme controls appearance of widgets and shapes
 * Themes are added to the ThemeRepository. You can add your own external themes.
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
     * @param dst is a Mat dedicated to hold the background will be filled with 4 channels
     * @param size will be the size of dst when done
     * @param color is the requested color, with alpha, which the theme is allowed to ignore
     */
    virtual void allocateBG(cv::Mat &dst, const cv::Size &size, const cv::Scalar &color) = 0;

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

/** @example example_add_theme.cpp
 * This is an example of how to add a theme.
 */

#endif // THEME_H
