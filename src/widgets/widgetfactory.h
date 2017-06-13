#ifndef WIDGETFACTORY_H
#define WIDGETFACTORY_H

#include <opencv2/core.hpp>
#include <map>
#include <functional>
#include "themes/themerepository.h"

namespace canvascv
{

using namespace std;
using namespace cv;

class Widget;

/**
 * @brief The WidgetFactory class
 * Widget prototypes are added to this class with the help of the WidgetFactoryT class.
 * You can create widgets with newWidget() of this class, but if you supply a wrong
 * name you'll get back a null pointer.
 */
class WidgetFactory
{
public:
    /**
     * @brief newWidget
     * Use this to create widgets by name.
     * @param type is the name of the concrete Widget sub type.
     * @param pos is an intial location for the Widget on the Layout
     * (if the layout supports specific positions).
     * @return a pointer to a newly allocated widget of type 'type' or nullptr if 'type' doesn't exist.
     */
    static Widget *newWidget(std::string type, const cv::Point &pos);

protected:
    typedef std::function<Widget*(const cv::Point &)> Allocator;
    static void addWidget(std::string name, Allocator a);

private:
    typedef map<std::string,std::function<Widget*(const cv::Point &)>> AllocatorsMap;
    static AllocatorsMap *allocators;
};

/**
 * @brief The WidgetFactoryT class
 * This is the compile-time type-safe version of the WidgetFactory.
 * Types added here with addType() are guranteed to exist at compile time.
 * Types created by newWidget() are guranteed to exist at compile time.
 */
template <class T>
class WidgetFactoryT : public WidgetFactory
{
public:
    /**
     * @brief addType adds type 'T' under name 'name'
     * @param name
     * @return true if type 'T' is registering for the first time ot false otherwise
     * @see REGISTER_WIDGET macro in WidgetFactory.h
     */
    static bool addType(std::string name);

    /**
     * @brief newWidget
     * Use this to create widgets by compile-time type.
     * @param pos is an intial location for the Widget on the Layout
     * (if the layout supports specific positions).
     * @return a pointer to a newly allocated widget of type 'type'.
     */
    static T *newWidget(const cv::Point &pos = cv::Point(0,0));
};

template <class T>
bool WidgetFactoryT<T>::addType(std::string name)
{
    static bool doneOnce;
    if (! doneOnce)
    {
        addWidget(name, [](const cv::Point& pos)->Widget*{ return new T(pos);});
        doneOnce = true;
    }
    return true;
}

template <class T>
T *WidgetFactoryT<T>::newWidget(const cv::Point &pos)
{
   T *shape = new T(pos);
   ThemeRepository::applyCurrentTheme(shape);
   return shape;
}

}

#define REGISTER_WIDGET(X) bool regWidget##X = canvascv::WidgetFactoryT<X>::addType(#X)


#endif // WIDGETFACTORY_H
