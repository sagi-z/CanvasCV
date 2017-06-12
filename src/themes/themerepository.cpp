#include "themerepository.h"
#include "theme.h"
#include <cstring>

using namespace std;

namespace canvascv
{

ThemeRepository::ThemeMap ThemeRepository::themes;
string ThemeRepository::currentThemeName;
Theme *ThemeRepository::currentTheme;

bool ThemeRepository::addTheme(const string &name, Theme *theme)
{
    if (theme)
    {
        themes[name] = theme;
        if (! currentTheme)
        {
            currentThemeName = name;
            currentTheme = theme;
        }
        return true;
    }
    return false;
}

void ThemeRepository::setCurrentTheme(const string &name)
{
    if (themes.find(name) != themes.end())
    {
        currentThemeName = name;
        currentTheme = themes[name];
    }
}

Theme *ThemeRepository::getCurrentTheme()
{
    return currentTheme;
}

string ThemeRepository::getCurrentThemeName()
{
   return currentThemeName;
}

void ThemeRepository::applyCurrentTheme(Shape *shape)
{
    if (currentTheme)
    {
        currentTheme->applyStyle(shape);
    }
}

void ThemeRepository::applyCurrentTheme(Widget *widget)
{
    if (currentTheme)
    {
        currentTheme->applyStyle(widget);
    }
}

}

// add include files here if needed
#include "themedark.h"

namespace canvascv
{

REGISTER_THEME(ThemeDark);

}
