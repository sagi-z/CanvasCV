#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include <opencv2/core.hpp>
#include <map>
#include <functional>

#include "canvascv/themes/themerepository.h"

namespace canvascv
{

using namespace std;
using namespace cv;

class Shape;

/**
 * @brief The ShapeFactory class
 * 
 * Holds shape creation methods. Don't use directly. Use Canvas::createShape() instead.
 * @sa Canvas::createShape()
 */
class ShapeFactory
{
public:
    /// create a shape by type name at a certain initial pos (don't use directly. Use Canvas::createShape() instead).
    static Shape *newShape(std::string type, const cv::Point &pos);
protected:
    typedef std::function<Shape*(const cv::Point &)> Allocator;
    static void addShape(std::string name, Allocator a);

private:
    typedef map<std::string,Allocator> AllocatorsMap;
    static AllocatorsMap *allocators;
};

/**
 * @brief The ShapeFactoryT class
 * 
 * Holds shape creation methods. Don't use directly. Use Canvas::createShape() instead.
 * @sa Canvas::createShape()
 */
template <class T>
class ShapeFactoryT : public ShapeFactory
{
public:
    /// add a shape by compile time type - use the macro REGISTER_SHAPE
    static bool addType(std::string name);

    /// create a shape by compile time type at a certain initial pos (don't use directly. Use Canvas::createShape() instead)
    static T *newShape(const cv::Point &pos);
};

template <class T>
bool ShapeFactoryT<T>::addType(std::string name)
{
    static bool doneOnce;
    if (! doneOnce)
    {
        addShape(name, [](const cv::Point& pos)->Shape*{ return new T(pos);});
        doneOnce = true;
    }
    return true;
}

template <class T>
T *ShapeFactoryT<T>::newShape(const cv::Point &pos)
{
   T *shape = new T(pos);
   ThemeRepository::applyCurrentTheme(shape);
   return shape;
}

}

/// add a shape to the factory in a way that it's compile time type matches it's string name
#define REGISTER_SHAPE(X) static bool regShape##X = canvascv::ShapeFactoryT<X>::addType(#X)


#endif // SHAPEFACTORY_H
