#ifndef THEMEREPOSITORY_H
#define THEMEREPOSITORY_H

#include <functional>
#include <map>
#include <string>

namespace canvascv
{

class Widget;
class Shape;

class ThemeRepository
{
public:
    typedef std::function<void(Shape*)> ShapeStyler;
    typedef std::function<void(Widget*)> WidgetStyler;
    typedef std::pair<ShapeStyler, WidgetStyler> ThemePair;
    typedef std::map<std::string,ThemePair> ThemeMap;

    static bool addTheme(const std::string &name, ShapeStyler shapeStyler);
    static bool addTheme(const std::string &name, WidgetStyler widgetStyler);
    static bool addTheme(const std::string &name, ShapeStyler shapeStyler, WidgetStyler widgetStyler);
    static void setCurrentTheme(const std::string &name);
    static std::string getCurrentTheme();
    static void applyCurrentTheme(Shape *shape);
    static void applyCurrentTheme(Widget *widget);
private:
    static ThemeMap themes;
    static std::string currentThemeName;
    static ThemePair *currentTheme;
};

}

#define REGISTER_THEME(NAME, S, W) bool regTheme##X = canvascv::ThemeRepository::addTheme(#NAME, S, W)

#endif // THEMEREPOSITORY_H
