#include "themerepository.h"
#include "theme.h"
#include <cstring>

// add theme include files here if needed
#include "themedarkorange.h"
#include "themedarkblue.h"

using namespace std;

namespace canvascv
{

ThemeRepository *ThemeRepository::instance()
{
   static ThemeRepository *inst = new ThemeRepository();
   return inst;
}

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

vector<string> ThemeRepository::availThemes()
{
    vector<string> result;
    for (auto &item : themes)
    {
        result.push_back(item.first);
    }
    return result;
}

#define REGISTER_THEME(X) canvascv::ThemeRepository::addTheme(#X, new X())

ThemeRepository::ThemeRepository()
    :currentTheme(nullptr)
{
    REGISTER_THEME(ThemeDarkOrange);
    REGISTER_THEME(ThemeDarkBlue);
}

}
