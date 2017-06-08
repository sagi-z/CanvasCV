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

class WidgetFactory
{
public:
    typedef std::function<Widget*(const cv::Point &)> Allocator;
    typedef map<std::string,std::function<Widget*(const cv::Point &)>> AllocatorsMap;

    static Widget *newWidget(std::string type, const cv::Point &pos);
protected:
    static void addWidget(std::string name, Allocator a);

private:
    static AllocatorsMap *allocators;
};

template <class T>
class WidgetFactoryT : public WidgetFactory
{
public:
    static bool addType(std::string name);
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
