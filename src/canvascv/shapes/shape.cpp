#include "shape.h"
#include "shapefactory.h"
#include "canvascv/canvas.h"

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

Shape::Shape()
    : id(genId()),
      outlineColor(Consts::DEFAULT_FG_COLOR),
      fillColor(outlineColor),
      locked(false),
      visible(true),
      editing(false),
      thickness(Consts::DEFAULT_THICKNESS),
      lineType(cv::LINE_AA),
      canvas(nullptr),
      deleted(false),
      ready(false)
{}

Shape::Shape(const Shape &other)
    : id(genId()),
      outlineColor(other.outlineColor),
      fillColor(other.fillColor),
      locked(other.locked),
      visible(other.visible),
      editing(other.editing),
      thickness(other.thickness),
      lineType(other.lineType),
      canvas(other.canvas),
      deleted(other.deleted),
      ready(other.ready)
{}

Shape::~Shape()
{
}

void Shape::notifyOnEvent(Shape::CBPerShape cb)
{
    cbs.push_back(cb);
}

Scalar Shape::getOutlineColor() const
{
    return outlineColor;
}

void Shape::setOutlineColor(const Scalar &value)
{
    outlineColor = value;
    outlineColor[3] = 255; // shape colors are opaque
}

Scalar Shape::getFillColor() const
{
    return fillColor;
}

void Shape::setFillColor(const Scalar &value)
{
    fillColor = value;
    fillColor[3] = 255; // shape colors are opaque
}

bool Shape::getLocked() const
{
    return locked;
}

void Shape::setLocked(bool value)
{
    locked = value;
}

bool Shape::getVisible() const
{
    return visible;
}

void Shape::setVisible(bool value)
{
    visible = value;
}

int Shape::getThickness() const
{
    return thickness;
}

void Shape::setThickness(int value)
{
    thickness = value;
}

int Shape::getLineType() const
{
    return lineType;
}

void Shape::setLineType(int value)
{
    lineType = value;
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
    const static string lockedMsg = "Shape is locked.";
    if (locked)
    {
        return lockedMsg;
    }
    else
    {
        if (isReady())
        {
            return getEditStatusMsg();
        }
        else
        {
            return getCreateStatusMsg();
        }
    }
}

void Shape::setDeleted()
{
    deleted = true;
}

bool Shape::isDeleted()
{
    return deleted;
}

void Shape::setReady()
{
    ready = true;
}

bool Shape::isReady() const
{
   return ready;
}

int Shape::getId()
{
    return id;
}

std::shared_ptr<Shape> Shape::getShape(int id)
{
    return nullptr;
}

bool Shape::keyPressed(int &key)
{
    (void)key; // remove compile warning;
    // consume nothing and keep focus for any key press by default
    return true;
}

void Shape::lostFocus()
{
    // do nothing by default
}

bool Shape::isEditing()
{
    return editing;
}

void Shape::broadcastEvent(Event event)
{
    for (auto &cb : cbs)
    {
        cb(this, event);
    }
}

int Shape::genId()
{
    static int idGenerator;
    return ++idGenerator;
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
    node["ready"] >> ready;
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
          "lineType" << lineType <<
          "ready" << ready;
}

ostream &operator<<(ostream &o, const Shape &shape)
{
    FileStorage fs("ignore.xml", FileStorage::WRITE | FileStorage::MEMORY);
    fs << shape.getType() << shape;
    o << fs.releaseAndGetString().c_str();
    return o;
}

}
