#include "themerepository.h"
#include <cstring>

using namespace std;

namespace canvascv
{

ThemeRepository::ThemeMap ThemeRepository::themes;
string ThemeRepository::currentThemeName;
ThemeRepository::ThemePair *ThemeRepository::currentTheme;

bool ThemeRepository::addTheme(const string &name, ShapeStyler shapeStyler)
{
    if (shapeStyler)
    {
        ThemePair &theme = themes[name];
        theme.first = shapeStyler;
        return true;
    }
    return false;
}

bool ThemeRepository::addTheme(const string &name, WidgetStyler widgetStyler)
{
    if (widgetStyler)
    {
        ThemePair &theme = themes[name];
        theme.second = widgetStyler;
        return true;
    }
    return false;
}

bool ThemeRepository::addTheme(const string &name, ThemeRepository::ShapeStyler shapeStyler, ThemeRepository::WidgetStyler widgetStyler)
{
   return addTheme(name, shapeStyler) && addTheme(name, widgetStyler);
}

void ThemeRepository::setCurrentTheme(const string &name)
{
    if (themes.find(name) != themes.end())
    {
        currentThemeName = name;
        currentTheme = &themes[name];
    }
}

string ThemeRepository::getCurrentTheme()
{
    return currentThemeName;
}

void ThemeRepository::applyCurrentTheme(Shape *shape)
{
    if (! currentTheme) setCurrentTheme("Default");
    if (currentTheme)
    {
        if (currentTheme->first) currentTheme->first(shape);
    }
}

void ThemeRepository::applyCurrentTheme(Widget *widget)
{
    if (currentTheme)
    {
        if (currentTheme->second) currentTheme->second(widget);
    }
}

}

// add include files here if needed
#include "colors.h"
#include "shape.h"
#include "widget.h"
#include "handle.h"
#include "button.h"

namespace canvascv
{

REGISTER_THEME(Default, [](Shape *shape) {
    if (shape->getType() == Handle::type)
    {
        shape->setOutlineColor(Colors::RED);
        shape->setFillColor(Colors::RED);
    }
    else
    {
        shape->setOutlineColor(Colors::BLACK);
        shape->setFillColor(Colors::P1_GRAY);
    }
}, [](Widget *widget) {
    if (widget->getType() == Button::type)
    {
        widget->setOutlineColor(Colors::BLACK);
        widget->setFillColor(Colors::BLACK);
    }
    else
    {
        widget->setOutlineColor(Colors::BLACK);
        widget->setFillColor(Colors::P1_GRAY);
    }
});

}
