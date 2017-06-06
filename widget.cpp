#include "widget.h"
#include "widgetfactory.h"
#include "layout.h"

namespace canvascv
{

void write(cv::FileStorage& fs, const std::string&, const Widget& x)
{
    x.write(fs);
}

void read(const cv::FileNode& node, Widget*& x, const Widget *default_value)
{
    (void)default_value; // avoid unused compilation warning
    if(node.empty())
    {
        x = 0;
    } else {
        std::string type = (std::string)node["XXXconcreteTypeXXX"];
        x = WidgetFactory::newWidget(type,cv::Point(0,0));
        x->read(node);
    }
}

Widget::Widget(const Point &pos)
    : id(genId()),
      leftPos(pos),
      outlineColor(Colors::GREEN),
      fillColor(outlineColor),
      locked(false),
      visible(true),
      thickness(1),
      lineType(cv::LINE_AA),
      forcedWidth(0),
      forcedHeight(0),
      anchor(TOP_LEFT),
      layout(nullptr),
      state(LEAVE),
      isDirty(false)
{}

Widget::Widget(const Widget &other)
    : id(genId()),
      leftPos(other.leftPos),
      outlineColor(other.outlineColor),
      fillColor(other.fillColor),
      locked(other.locked),
      visible(other.visible),
      thickness(other.thickness),
      lineType(other.lineType),
      forcedWidth(other.forcedWidth),
      forcedHeight(other.forcedHeight),
      anchor(other.anchor),
      layout(other.layout),
      state(LEAVE),
      isDirty(other.isDirty)
{}

Widget::~Widget()
{
}

void Widget::notifyOnChange(Widget::CBType cb)
{
    changeNotifs.push_back(cb);
}

Scalar Widget::getOutlineColor() const
{
    return outlineColor;
}

void Widget::setOutlineColor(const Scalar &value)
{
    if (outlineColor != value)
    {
        outlineColor = value;
        setDirty();
    }
}

Scalar Widget::getFillColor() const
{
    return fillColor;
}

void Widget::setFillColor(const Scalar &value)
{
    if (fillColor != value)
    {
        fillColor = value;
        setDirty();
    }
}



void Widget::setLocked(bool value)
{
    locked = value;
}

void Widget::setVisible(bool value)
{
    visible = value;
}

int Widget::getThickness() const
{
    return thickness;
}

void Widget::setThickness(int value)
{
    if (thickness != value)
    {
        thickness = value;
        setDirty();
    }
}

int Widget::getLineType() const
{
    return lineType;
}

void Widget::setLineType(int value)
{
    if (lineType != value)
    {
        lineType = value;
        setDirty();
    }
}

Widget::Anchor Widget::getAnchor() const
{
    return anchor;
}

void Widget::setAnchor(const Widget::Anchor &value)
{
    if (anchor != value)
    {
        anchor = value;
        setDirty();
    }
}

bool Widget::isAtPos(const Point &pos)
{
    return getRect().contains(pos);
}

void Widget::setLayout(Layout &value)
{
    if (layout != &value)
    {
        layout = &value;

        // Verify we're dirty in the new layout
        isDirty = false;
        setDirty();
    }
}

Layout *Widget::getLayout()
{
    return layout;
}

const string &Widget::getStatusMsg() const
{
    return statusMsg;
}

void Widget::setStatusMsg(const std::string &value)
{
    statusMsg = value;
}

void Widget::broadcastChange(State status)
{
    state = status;
    switch (state)
    {
    case LEAVE:
        mouseLeave();
        break;
    case ENTER:
        mouseEnter();
        break;
    case PRESS:
        mousePressed();
        break;
    case RELEASE:
        mouseReleased();
        break;
    }

    for (auto &cb : changeNotifs)
    {
        cb(this, status);
    }
}

void Widget::write(cv::FileStorage& fs) const
{
    fs << "{";
    writeInternals(fs);
    fs << "}";
}

void Widget::read(const cv::FileNode& node)
{
    readInternals(node);
}

Widget::State Widget::getState() const
{
    return state;
}

void Widget::stretchWidth(int width)
{
   if (width != forcedWidth)
   {
       forcedWidth = width;
       setDirty();
   }
}

void Widget::stretchHeight(int height)
{
   if (height != forcedHeight)
   {
       forcedHeight = height;
       setDirty();
   }
}

int Widget::genId()
{
    static int idGenerator;
    return ++idGenerator;
}

bool Widget::getIsDirty() const
{
    return isDirty;
}

cv::Point Widget::getLeftPos() const
{
    return leftPos;
}

void Widget::translate(const Point &translation)
{
    leftPos += translation;
}

void Widget::setDirty()
{
    if (! isDirty)
    {
        if (layout)
        {
            isDirty = true;
            layout->addDirtyWidget(this);
        }
    }
}

void Widget::update()
{
   if (isDirty)
   {
       isDirty = false;
       recalc();
   }
}

void Widget::setLeftPos(const Point &value)
{
    Point translation = value - leftPos;
    translate(translation);
}

void Widget::readInternals(const cv::FileNode &node)
{
    node["id"] >> id;
    node["leftPos"] >> leftPos;
    node["outlineColor"] >> outlineColor;
    node["fillColor"] >> fillColor;
    node["locked"] >> locked;
    node["visible"] >> visible;
    node["thickness"] >> thickness;
    node["lineType"] >> lineType;
    node["anchor"] >> (int) anchor;
    node["statusMsg"] >> statusMsg;
    state = LEAVE;
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
    setDirty();
}

void Widget::writeInternals(cv::FileStorage &fs) const
{
    fs << "XXXconcreteTypeXXX" << getType() <<
          "id" << id <<
          "leftPos" << leftPos <<
          "outlineColor" << outlineColor <<
          "fillColor" << fillColor <<
          "locked" << locked <<
          "visible" << visible <<
          "thickness" << thickness <<
          "lineType" << lineType <<
          "anchor" << anchor <<
          "statusMsg" << statusMsg;
}

std::ostream &operator<<(std::ostream &o, const Widget &shape)
{
    cv::FileStorage fs("ignore.xml", cv::FileStorage::WRITE | cv::FileStorage::MEMORY);
    fs << shape.getType() << shape;
    o << fs.releaseAndGetString().c_str();
    return o;
}

}
