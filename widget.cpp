#include "widget.h"
#include "widgetfactory.h"
#include "canvas.h"

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

Widget::~Widget()
{
}

void Widget::notifyOnChange(Widget::CBType cb)
{
    changeNotifs.push_back(cb);
}

void Widget::setCanvas(Canvas &value)
{
    canvas = &value;
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

void Widget::readInternals(const cv::FileNode &node)
{
    node["id"] >> id;
    node["outlineColor"] >> outlineColor;
    node["fillColor"] >> fillColor;
    node["locked"] >> locked;
    node["visible"] >> visible;
    node["thickness"] >> thickness;
    node["lineType"] >> lineType;
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
}

void Widget::writeInternals(cv::FileStorage &fs) const
{
    fs << "XXXconcreteTypeXXX" << getType() <<
          "id" << id <<
          "outlineColor" << outlineColor <<
          "fillColor" << fillColor <<
          "locked" << locked <<
          "visible" << visible <<
          "thickness" << thickness <<
          "lineType" << lineType <<
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
