#ifndef THEMEREPOSITORY_H
#define THEMEREPOSITORY_H

#include <functional>
#include <map>
#include <vector>
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
    static ThemeRepository * instance();

    /**
     * @brief addTheme add a Theme to the repository
     * @param name is the name of the theme (to overide a theme, add something instead with the same name)
     * @param theme is the theme associated with this name
     * @return true if theme is not null
     * @note that you cannot delete the Theme pointer while it's in use.
     * @see setCurrentTheme()
     */
    bool addTheme(const std::string &name, Theme *theme);

    /**
     * @brief setCurrentTheme sets a previously added theme
     * @param name is the name of the Theme to set as default
     * @note if the theme doesn't exist then nothing happens
     */
    void setCurrentTheme(const std::string &name);

    /// returns the current theme
    Theme *getCurrentTheme();

    /// returns the current theme name
    std::string getCurrentThemeName();

    /// used automatically by the ShapeFactory when creating a shape
    void applyCurrentTheme(Shape *shape);

    /// used automatically by the WidgetFactory when creating a widget
    void applyCurrentTheme(Widget *widget);

    /// get a list of available theme names
    std::vector<std::string> availThemes();

private:
    ThemeRepository();

    typedef std::map<std::string,Theme *> ThemeMap;

    ThemeMap themes;
    std::string currentThemeName;
    Theme *currentTheme;
};

}

#endif // THEMEREPOSITORY_H
