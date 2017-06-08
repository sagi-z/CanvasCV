#include "shape.h"
#include "shapefactory.h"
#include "canvas.h"

using namespace std;
using namespace cv;

namespace canvascv
{

void write(FileStorage& fs, const string&, const Shape& x)
{
    x.write(fs);
}

void read(const FileNode& node, Shape*& x, const Shape *default_value)
{
    (void)default_value; // avoid unused compilation warning
    if(node.empty())
    {
        x = 0;
    } else {
        string type = (string)node["XXXconcreteTypeXXX"];
        x = ShapeFactory::newShape(type,Point(0,0));
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

void Shape::drawHelper(Mat &canvas, Shape *other)
{
   other->draw(canvas);
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

void Shape::setDeleted()
{
    deleted = true;
}

bool Shape::isDeleted()
{
   return deleted;
}

void Shape::broadcastEvent(CBEvent event)
{
    for (auto &cb : cbs)
    {
        cb(this, event);
    }
}

void Shape::write(FileStorage& fs) const
{
    fs << "{";
    writeInternals(fs);
    fs << "}";
}

void Shape::read(const FileNode& node)
{
    readInternals(node);
}

void Shape::readInternals(const FileNode &node)
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

void Shape::writeInternals(FileStorage &fs) const
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

ostream &operator<<(ostream &o, const Shape &shape)
{
    FileStorage fs("ignore.xml", FileStorage::WRITE | FileStorage::MEMORY);
    fs << shape.getType() << shape;
    o << fs.releaseAndGetString().c_str();
    return o;
}

}
