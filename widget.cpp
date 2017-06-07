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
      location(pos),
      outlineColor(Colors::GREEN),
      fillColor(outlineColor),
      locked(false),
      visible(true),
      thickness(1),
      lineType(cv::LINE_AA),
      alpha(0.5),
      forcedWidth(0),
      forcedHeight(0),
      layoutAnchor(TOP_LEFT),
      flowAnchor(TOP_LEFT),
      stretchX(false),
      stretchY(false),
      layout(nullptr),
      state(LEAVE),
      isDirty(false),
      delayedUpdate(true)
{}

Widget::Widget(const Widget &other)
    : id(genId()),
      location(other.location),
      outlineColor(other.outlineColor),
      fillColor(other.fillColor),
      locked(other.locked),
      visible(other.visible),
      thickness(other.thickness),
      lineType(other.lineType),
      alpha(other.alpha),
      forcedWidth(other.forcedWidth),
      forcedHeight(other.forcedHeight),
      flowAnchor(other.layoutAnchor),
      stretchX(other.stretchX),
      stretchY(other.stretchY),
      layout(other.layout),
      state(LEAVE),
      isDirty(other.isDirty),
      delayedUpdate(true)
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

double Widget::getAlpha() const
{
    return alpha;
}

void Widget::setAlpha(double value)
{
    if (alpha != value)
    {
        alpha = value;
        setDirty();
    }
}

void Widget::setLayoutAnchor(const Widget::Anchor &value)
{
    if (layoutAnchor != value)
    {
        layoutAnchor = value;
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

void Widget::layoutResized(const cv::Rect &boundaries)
{
   setDirty();
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

bool Widget::getStretchY() const
{
    return stretchY;
}

void Widget::setStretchY(bool value)
{
    if (stretchY != value)
    {
        stretchY = value;
        setDirty();
    }
}

bool Widget::getStretchX() const
{
    return stretchX;
}

void Widget::setStretchX(bool value)
{
    if (stretchX != value)
    {
        stretchX = value;
        setDirty();
    }
}

void Widget::setFlowAnchor(const Anchor &value)
{
    if (flowAnchor != value)
    {
        flowAnchor = value;
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

cv::Point Widget::getLocation() const
{
    return location;
}

void Widget::translate(const Point &translation)
{
    location += translation;
}

void Widget::setDirty()
{
    if (delayedUpdate)
    {
        if (! isDirty)
        {
            if (layout && layout->addDirtyWidget(this))
            {
                isDirty = true;
                return;
            }
            else
            {
                isDirty = false;
                recalc();
            }
        }
        else
        {
            // already dirty - do nothing
        }
    }
    else
    {
        isDirty = false;
        recalc();
    }
}

void Widget::update()
{
   if (isDirty)
   {
       delayedUpdate = false;
       isDirty = false;
       recalc();
       delayedUpdate = true;
   }
}

void Widget::setLocation(const Point &value)
{
    Point translation = value - location;
    translate(translation);
}

void Widget::readInternals(const cv::FileNode &node)
{
    node["id"] >> id;
    node["leftPos"] >> location;
    node["outlineColor"] >> outlineColor;
    node["fillColor"] >> fillColor;
    node["locked"] >> locked;
    node["visible"] >> visible;
    node["thickness"] >> thickness;
    node["lineType"] >> lineType;
    node["alpha"] >> alpha;
    node["layoutAnchor"] >> (int) layoutAnchor;
    node["flowAnchor"] >> (int) flowAnchor;
    node["stretchX"] >> stretchX;
    node["stretchY"] >> stretchY;
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
          "leftPos" << location <<
          "outlineColor" << outlineColor <<
          "fillColor" << fillColor <<
          "locked" << locked <<
          "visible" << visible <<
          "thickness" << thickness <<
          "lineType" << lineType <<
          "alpha" << alpha <<
          "layoutAnchor" << layoutAnchor <<
          "flowAnchor" << flowAnchor <<
          "stretchX" << stretchX <<
          "stretchY" << stretchY <<
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
