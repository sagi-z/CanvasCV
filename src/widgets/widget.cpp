#include "widget.h"
#include "widgetfactory.h"
#include "layout.h"
#include "themes/theme.h"
#include "themes/themerepository.h"

using namespace std;
using namespace cv;

namespace canvascv
{

/* TODO - write/read widgets to file for a designer app
void write(FileStorage& fs, const string&, const Widget& x)
{
    x.write(fs);
}

void read(const FileNode& node, Widget*& x, const Widget *default_value)
{
    (void)default_value; // avoid unused compilation warning
    if(node.empty())
    {
        x = 0;
    } else {
        string type = (string)node["XXXconcreteTypeXXX"];
        x = WidgetFactory::newWidget(type,Point(0,0));
        x->read(node);
    }
}
*/

Widget::Widget(const Point &pos)
    : id(genId()),
      location(pos),
      outlineColor(Colors::Green),
      fillColor(Colors::Black),
      selectColor(Colors::Orange),
      relief(FLAT),
      visible(true),
      thickness(1),
      lineType(LINE_AA),
      forcedWidth(0),
      forcedHeight(0),
      layoutAnchor(TOP_LEFT),
      flowAnchor(TOP_LEFT),
      stretchX(false),
      stretchY(false),
      isSelectable(false),
      layout(nullptr),
      state(LEAVE),
      isDirty(false),
      updateCalls(0)
{
    outlineColor[3] = 255; // FG is always opaque
    selectColor[3] = 255; // select is opaque
    fillColor[3] = 128; // BG is semi transparent by default
}

Widget::~Widget()
{
}

void Widget::notifyOnChange(Widget::CBWidgetState cb)
{
    changeNotifs.push_back(cb);
}

Scalar Widget::getOutlineColor() const
{
    return outlineColor;
}

void Widget::setOutlineColor(const Scalar &value)
{
    if (outlineColor[0] != value[0] ||
            outlineColor[1] != value[1] ||
            outlineColor[2] != value[2])
    {
        outlineColor= value;
        outlineColor[3] = 255;
        setDirty();
    }
}

Scalar Widget::getFillColor() const
{
    return fillColor;
}

void Widget::setFillColor(const Scalar &value)
{
    if (fillColor[0] != value[0] ||
            fillColor[1] != value[1] ||
            fillColor[2] != value[2])
    {
        fillColor[0] = value[0];
        fillColor[1] = value[1];
        fillColor[2] = value[2];
        setDirty();
    }
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

uchar Widget::getAlpha() const
{
    return (uchar) fillColor[3];
}

void Widget::setAlpha(uchar value)
{
    if (fillColor[3] !=  value)
    {
        fillColor[3] = value;
        setDirty();
    }
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

void Widget::layoutResized(const Rect &boundaries)
{
   setDirty();
}

const string &Widget::getStatusMsg() const
{
    return statusMsg;
}

void Widget::setStatusMsg(const string &value)
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
void Widget::write(FileStorage& fs) const
{
    fs << "{";
    writeInternals(fs);
    fs << "}";
}

void Widget::read(const FileNode& node)
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
    setForcedWidth(width);
}

void Widget::stretchHeight(int height)
{
    setForcedHeight(height);
}

int Widget::getForcedHeight() const
{
    return forcedHeight;
}

void Widget::setForcedHeight(int value)
{
    if (forcedHeight != value)
    {
        forcedHeight = value;
        setDirty();
    }
}

int Widget::getForcedWidth() const
{
    return forcedWidth;
}

void Widget::setForcedWidth(int value)
{
    if (forcedWidth != value)
    {
        forcedWidth = value;
        setDirty();
    }
}

bool Widget::getIsSelectable() const
{
    return isSelectable;
}

void Widget::setIsSelectable(bool value)
{
    isSelectable = value;
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

Rect Widget::getLayoutBoundaries() const
{
    if (layout)
    {
        return layout->getBoundaries();
    }
    else
    {
        return Rect();
    }
}

Scalar Widget::getSelectColor() const
{
    return selectColor;
}

void Widget::setSelectColor(const Scalar &value)
{
    if (selectColor[0] != value[0] ||
            selectColor[1] != value[1] ||
            selectColor[2] != value[2])
    {
        selectColor = value;
        selectColor[3] = 255;
        setDirty();
    }
}

void Widget::mergeMats(Mat &roiSrc, Mat &roiDst)
{
    assert(roiSrc.size() == roiDst.size());
    if (roiSrc.channels() == 3)
    {
        assert(roiDst.channels() == 4); // that case is not implemented yet
        Vec3b *pSrcRow;
        Vec4b *pDstRow;
        for (int r = 0; r < roiDst.rows; ++r)
        {
            pSrcRow = roiSrc.ptr<Vec3b>(r);
            pDstRow = roiDst.ptr<Vec4b>(r);
            for (int c = 0; c < roiDst.cols; ++c)
            {
                pDstRow[c][0] = pSrcRow[c][0];
                pDstRow[c][1] = pSrcRow[c][1];
                pDstRow[c][2] = pSrcRow[c][2];
                pDstRow[c][3] = 255; // opaque
            }
        }
    }
    else
    {
        if (roiDst.channels() == 4)
        {
            Vec4b *pSrcRow;
            Vec4b *pDstRow;
            double alpha, beta;
            for (int r = 0; r < roiDst.rows; ++r)
            {
                pSrcRow = roiSrc.ptr<Vec4b>(r);
                pDstRow = roiDst.ptr<Vec4b>(r);
                for (int c = 0; c < roiDst.cols; ++c)
                {
                    alpha = pSrcRow[c][3] / 255.;
                    beta = 1. - alpha;
                    pDstRow[c][0] = (uchar) pSrcRow[c][0]*alpha + pDstRow[c][0]*beta;
                    pDstRow[c][1] = (uchar) pSrcRow[c][1]*alpha + pDstRow[c][1]*beta;
                    pDstRow[c][2] = (uchar) pSrcRow[c][2]*alpha + pDstRow[c][2]*beta;
                }
            }
        }
        else
        {
            Vec4b *pSrcRow;
            Vec3b *pDstRow;
            double alpha, beta;
            for (int r = 0; r < roiDst.rows; ++r)
            {
                pSrcRow = roiSrc.ptr<Vec4b>(r);
                pDstRow = roiDst.ptr<Vec3b>(r);
                for (int c = 0; c < roiDst.cols; ++c)
                {
                    alpha = pSrcRow[c][3] / 255.;
                    beta = 1. - alpha;
                    pDstRow[c][0] = (uchar) pSrcRow[c][0]*alpha + pDstRow[c][0]*beta;
                    pDstRow[c][1] = (uchar) pSrcRow[c][1]*alpha + pDstRow[c][1]*beta;
                    pDstRow[c][2] = (uchar) pSrcRow[c][2]*alpha + pDstRow[c][2]*beta;
                }
            }
        }
    }
}

void Widget::paintRelief()
{
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
    case SELECTED:
        ThemeRepository::getCurrentTheme()->selected(bg, fillColor);
        break;
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
    if (isSelectable)
    {
        selectedWidget();
    }
}

void Widget::mouseLeave()
{
    paintRelief();
}

void Widget::allocateBG(const Size &size)
{
    ThemeRepository::getCurrentTheme()->allocateBG(bg, size, fillColor);
    paintRelief();
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

void Widget::renderOn(Mat &dst)
{
    const Rect &rect = getRect();
    if (rect.width && rect.height)
    {
        Rect dstRect({0,0}, dst.size());
        Rect intersection = rect & dstRect;
        if (intersection.width && intersection.height)
        {
            Mat roiDst(dst, intersection);
            if (! bg.empty())
            {
                Mat roiSrcBG(bg, Rect(intersection.x - rect.x,
                                      intersection.y - rect.y,
                                      intersection.width, intersection.height));
                mergeMats(roiSrcBG, roiDst);
            }
            if (! fg.empty())
            {
                Mat roiSrcFG(fg, Rect(intersection.x - rect.x,
                                      intersection.y - rect.y,
                                      intersection.width, intersection.height));
                mergeMats(roiSrcFG, roiDst);
            }
        }
    }
}

void Widget::callDrawFG(bool preAllocateMat)
{
    const Rect &rect = getRect();
    if (rect.width && rect.height)
    {
        if (preAllocateMat)
        {
            fg = Mat::zeros(rect.height, rect.width, CV_8UC4);
        }
        drawFG(fg);
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

shared_ptr<Widget> Widget::rmvFromLayout()
{
    shared_ptr<Widget> result;
    if (layout)
    {
        result = layout->rmvWidget(this);
        layout = nullptr;
    }
    return result;
}

bool Widget::getIsDirty() const
{
    return isDirty;
}

Point Widget::getLocation() const
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

bool Widget::setDirty()
{
    if (! isDirty)
    {
        if (layout)
        {
            if (layout->addDirtyWidget(this))
            {
                isDirty = true; // only if we're in a dirty layout
            }
        }
    }
    return true;
}

void Widget::update()
{
    isDirty = false;
    recalc();
    isDirty = false;
}

bool Widget::isRemoved() const
{
    return layout == nullptr;
}

void Widget::setLocation(const Point &value)
{
    Point translation = value - location;
    translate(translation);
}

/* TODO - write/read widgets to file for a designer app
void Widget::readInternals(const FileNode &node)
{
    node["id"] >> id;
    node["leftPos"] >> location;
    node["outlineColor"] >> outlineColor;
    node["fillColor"] >> fillColor;
    node["visible"] >> visible;
    node["thickness"] >> thickness;
    node["lineType"] >> lineType;
    node["layoutAnchor"] >> (int) layoutAnchor;
    node["flowAnchor"] >> (int) flowAnchor;
    node["stretchX"] >> stretchX;
    node["stretchY"] >> stretchY;
    node["isSelectable"] >> isSelectable;
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

void Widget::writeInternals(FileStorage &fs) const
{
    fs << "XXXconcreteTypeXXX" << getType() <<
          "id" << id <<
          "leftPos" << location <<
          "outlineColor" << outlineColor <<
          "fillColor" << fillColor <<
          "visible" << visible <<
          "thickness" << thickness <<
          "lineType" << lineType <<
          "layoutAnchor" << layoutAnchor <<
          "flowAnchor" << flowAnchor <<
          "stretchX" << stretchX <<
          "stretchY" << stretchY <<
          "isSelectable" << isSelectable <<
          "statusMsg" << statusMsg;
}

ostream &operator<<(ostream &o, const Widget &widget)
{
    FileStorage fs("ignore.xml", FileStorage::WRITE | FileStorage::MEMORY);
    fs << widget.getType() << widget;
    o << fs.releaseAndGetString().c_str();
    return o;
}
*/

}
