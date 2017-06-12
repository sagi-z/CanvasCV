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

class ThemeRepository
{
public:
    typedef std::map<std::string,Theme *> ThemeMap;

    static bool addTheme(const std::string &name, Theme *theme);
    static void setCurrentTheme(const std::string &name);
    static Theme *getCurrentTheme();
    static std::string getCurrentThemeName();
    static void applyCurrentTheme(Shape *shape);
    static void applyCurrentTheme(Widget *widget);
private:

    static ThemeMap themes;
    static std::string currentThemeName;
    static Theme *currentTheme;
};

}

#define REGISTER_THEME(NAME) bool regTheme##X = canvascv::ThemeRepository::addTheme(#NAME, new NAME())

#endif // THEMEREPOSITORY_H
