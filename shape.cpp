#include "shape.h"
#include "shapefactory.h"
#include "canvas.h"

namespace canvascv
{

void write(cv::FileStorage& fs, const std::string&, const Shape& x)
{
    x.write(fs);
}

void read(const cv::FileNode& node, Shape*& x, const Shape *default_value)
{
    (void)default_value; // avoid unused compilation warning
    if(node.empty())
    {
        x = 0;
    } else {
        std::string type = (std::string)node["XXXconcreteTypeXXX"];
        x = ShapeFactory::newShape(type,cv::Point(0,0));
        x->read(node);
    }
}

Shape::~Shape()
{
}

void Shape::notifyOnEvent(Shape::CBType cb)
{
    cbs.push_back(cb);
}

void Shape::setCanvas(Canvas &value)
{
    canvas = &value;
}

const string &Shape::getStatusMsg() const
{
   static const string emptyStr;
   return emptyStr;
}

void Shape::broadcastEvent(CBEvent event)
{
    for (auto &cb : cbs)
    {
        cb(this, event);
    }
}

void Shape::write(cv::FileStorage& fs) const
{
    fs << "{";
    writeInternals(fs);
    fs << "}";
}

void Shape::read(const cv::FileNode& node)
{
    readInternals(node);
}

void Shape::readInternals(const cv::FileNode &node)
{
    node["id"] >> id;
    node["outlineColor"] >> outlineColor;
    node["fillColor"] >> fillColor;
    node["locked"] >> locked;
    node["visible"] >> visible;
    node["thickness"] >> thickness;
    node["lineType"] >> lineType;
    editing = false;
    if (id == 0)
    {
        // backward compatible for olde config files
        id = genId();
    }
    else
    {
        // ensure no duplicate ids.
        // new generated ids will always be bigger than ones in files.
        while (genId() < id) {}
    }
}

void Shape::writeInternals(cv::FileStorage &fs) const
{
    fs << "XXXconcreteTypeXXX" << getType() <<
          "id" << id <<
          "outlineColor" << outlineColor <<
          "fillColor" << fillColor <<
          "locked" << locked <<
          "visible" << visible <<
          "thickness" << thickness <<
          "lineType" << lineType;
}

std::ostream &operator<<(std::ostream &o, const Shape &shape)
{
    cv::FileStorage fs("ignore.xml", cv::FileStorage::WRITE | cv::FileStorage::MEMORY);
    fs << shape.getType() << shape;
    o << fs.releaseAndGetString().c_str();
    return o;
}

}
