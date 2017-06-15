#include "widget.h"
#include "widgetfactory.h"
#include "layout.h"
#include "themes/theme.h"
#include "themes/themerepository.h"

namespace canvascv
{

/* TODO - write/read widgets to file for a designer app
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
*/

Widget::Widget(Layout &layoutVal, const Point &pos)
    : id(genId()),
      location(pos),
      outlineColor(Colors::GREEN),
      fillColor(outlineColor),
      selectColor(Colors::P1_ORANGE),
      relief(FLAT),
      locked(false),
      visible(true),
      thickness(1),
      lineType(cv::LINE_AA),
      alpha(0.5),
      fillBG(true),
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
{
    shared_ptr<Widget> toBeReplacedOnFactoryCall(this, [](Widget*){});
    layoutVal.addWidget(toBeReplacedOnFactoryCall);
}

Widget::Widget(const Widget &other)
    : id(genId()),
      location(other.location),
      outlineColor(other.outlineColor),
      fillColor(other.fillColor),
      selectColor(other.selectColor),
      relief(other.relief),
      locked(other.locked),
      visible(other.visible),
      thickness(other.thickness),
      lineType(other.lineType),
      alpha(other.alpha),
      fillBG(other.fillBG),
      forcedWidth(other.forcedWidth),
      forcedHeight(other.forcedHeight),
      flowAnchor(other.layoutAnchor),
      stretchX(other.stretchX),
      stretchY(other.stretchY),
      layout(nullptr),
      state(LEAVE),
      isDirty(other.isDirty),
      delayedUpdate(true)
{
    shared_ptr<Widget> toBeReplacedOnFactoryCall(this, [](Widget*){});
    other.layout->addWidget(toBeReplacedOnFactoryCall);
}

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
    alpha = value; // this is only used during draw()
}

void Widget::setLayoutAnchor(const Anchor &value)
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

/* TODO - write/read widgets to file for a designer app
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
*/


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

Widget::Relief Widget::getRelief() const
{
    return relief;
}

void Widget::setRelief(const Relief &value)
{
    if (relief != value)
    {
        relief = value;
        setDirty();
    }
}

cv::Scalar Widget::getSelectColor() const
{
    return selectColor;
}

void Widget::setSelectColor(const cv::Scalar &value)
{
    if (selectColor != value)
    {
        selectColor = value;
        setDirty();
    }
}

void Widget::mousePressed()
{
}

void Widget::mouseReleased()
{
}

void Widget::mouseEnter()
{
}

void Widget::mouseLeave()
{
}

bool Widget::getFillBG() const
{
    return fillBG;
}

void Widget::setFillBG(bool value)
{
    if (fillBG != value)
    {
        fillBG = value;
        setDirty();
    }
}

void Widget::allocateBG(const Size &size, int type)
{
    ThemeRepository::getCurrentTheme()->allocateBG(bg, size, fillColor, type);
    switch (relief)
    {
    case FLAT:
        ThemeRepository::getCurrentTheme()->flat(bg, fillColor);
        break;
    case RAISED:
        ThemeRepository::getCurrentTheme()->raised(bg, fillColor);
        break;
    case SUNKEN:
        ThemeRepository::getCurrentTheme()->sunken(bg, fillColor);
        break;
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

void Widget::drawBG(Mat &dst, const Rect &rect)
{
    if (fillBG && ! bg.empty())
    {
        if (dst.type() != bg.type())
        {
            allocateBG(bg.size(), dst.type());
        }
        ThemeRepository::getCurrentTheme()->drawBG(dst, rect, bg, alpha);
    }
}

void Widget::flatWidget()
{
    ThemeRepository::getCurrentTheme()->flat(bg, fillColor);
}

void Widget::raisedWidget()
{
    ThemeRepository::getCurrentTheme()->raised(bg, fillColor);
}

void Widget::sunkenWidget()
{
    ThemeRepository::getCurrentTheme()->sunken(bg, fillColor);
}

void Widget::selectedWidget()
{
    ThemeRepository::getCurrentTheme()->selected(bg, selectColor);
}

void Widget::draw(Mat &dst)
{
    const Rect &rect = getRect();
    if (rect.width && rect.height)
    {
        drawBG(dst, rect);
        Mat roi = dst(rect);
        drawFG(roi);
    }
}

void Widget::drawFG(Mat &dst)
{
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
    if (translation.x != 0 || translation.y != 0)
    {
        location += translation;
        setDirty();
    }
}

void Widget::setDirty()
{
    if (delayedUpdate)
    {
        if (! isDirty)
        {
            if (layout->addDirtyWidget(this))
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

/* TODO - write/read widgets to file for a designer app
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
    node["fillBG"] >> fillBG;
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
          "fillBG" << fillBG <<
          "layoutAnchor" << layoutAnchor <<
          "flowAnchor" << flowAnchor <<
          "stretchX" << stretchX <<
          "stretchY" << stretchY <<
          "statusMsg" << statusMsg;
}

std::ostream &operator<<(std::ostream &o, const Widget &widget)
{
    cv::FileStorage fs("ignore.xml", cv::FileStorage::WRITE | cv::FileStorage::MEMORY);
    fs << widget.getType() << widget;
    o << fs.releaseAndGetString().c_str();
    return o;
}
*/

}
