#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include <opencv2/core.hpp>
#include <map>
#include <functional>

namespace canvasvc
{

using namespace std;
using namespace cv;

class Shape;

class ShapeFactory
{
public:
    typedef std::function<Shape*(const cv::Point &)> Allocator;
    typedef map<std::string,std::function<Shape*(const cv::Point &)>> AllocatorsMap;

    static Shape *newShape(std::string type, const cv::Point &pos);
protected:
    static void addShape(std::string name, Allocator a);

private:
    static AllocatorsMap *allocators;
};

template <class T>
class ShapeFactoryT : public ShapeFactory
{
public:
    static bool addType(std::string name);
    static Shape *newShape(const cv::Point &pos);
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
Shape *ShapeFactoryT<T>::newShape(const cv::Point &pos)
{
   Shape *shape = new T(pos);
   return shape;
}

}

#define REGISTER_SHAPE(X) bool regShape##X = canvasvc::ShapeFactoryT<X>::addType(#X)


#endif // SHAPEFACTORY_H
