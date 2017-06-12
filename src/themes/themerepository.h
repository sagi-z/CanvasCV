#ifndef THEMEREPOSITORY_H
#define THEMEREPOSITORY_H

#include <functional>
#include <map>
#include <string>

namespace canvascv
{

class Widget;
class Shape;
class Theme;

/**
 * @brief The ThemeRepository is used to add and get themes for Widget and Shape objects
 * This class must have at least a single theme. Themes can be a part of this library, or
 * external to it, suplied by the user and added with ThemeRepository::addTheme()
 */
class ThemeRepository
{
public:
    /**
     * @brief addTheme add a Theme to the repository
     * @param name is the name of the theme (to overide a theme, add something instead with the same name)
     * @param theme is the theme associated with this name
     * @return true if theme is not null
     * @note that you cannot delete the Theme pointer while it's in use.
     * @see setCurrentTheme()
     */
    static bool addTheme(const std::string &name, Theme *theme);

    /**
     * @brief setCurrentTheme sets a previously added theme
     * @param name is the name of the Theme to set as default
     * @note if the theme doesn't exist then nothing happens
     */
    static void setCurrentTheme(const std::string &name);

    /// returns the current theme
    static Theme *getCurrentTheme();

    /// returns the current theme name
    static std::string getCurrentThemeName();

    /// used automatically by the ShapeFactory when creating a shape
    static void applyCurrentTheme(Shape *shape);

    /// used automatically by the WidgetFactory when creating a widget
    static void applyCurrentTheme(Widget *widget);

private:

    typedef std::map<std::string,Theme *> ThemeMap;

    static ThemeMap themes;
    static std::string currentThemeName;
    static Theme *currentTheme;
};

}

#define REGISTER_THEME(NAME) bool regTheme##X = canvascv::ThemeRepository::addTheme(#NAME, new NAME())

#endif // THEMEREPOSITORY_H
