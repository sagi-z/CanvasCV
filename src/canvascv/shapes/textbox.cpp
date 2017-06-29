#include "textbox.h"
#include "canvascv/colors.h"

#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

namespace canvascv
{

const char * TextBox::type = "TextBox";

TextBox::TextBox(const Point &pos) :
    Shape(),
    text("text"),
    prevText(text),
    fontFace(Consts::DEFAULT_FONT),
    fontScale(Consts::DEFAULT_FONT_SCALE),
    fontThickness(Consts::DEFAULT_FONT_THICKNESS),
    fontColor(Consts::DEFAULT_FONT_COLOR)
{
    topLeft.reset(ShapeFactoryT<Handle>::newShape(pos));
    topLeft->setLocked(true);
    recalcRect();
    registerCBs();
    setReady();
}


void TextBox::draw(Mat &canvas)
{
    if (visible)
    {
        if (editing)
        {
            Rect rectSelected(rect);
            rectSelected.x -= 2;
            rectSelected.y -= 2;
            rectSelected.width += 4;
            rectSelected.height += 4;
            rectangle(canvas, rectSelected, fillColor, thickness);
        }
        rectangle(canvas, rect, outlineColor, -1);
        putText(canvas, text, Point(rect.tl().x,rect.tl().y+baseline*2), fontFace, fontScale,
                fontColor, fontThickness, LINE_AA);
        drawHelper(canvas, topLeft.get());
    }
}

bool TextBox::mousePressed(const Point &pos, bool onCreate)
{
    if (visible)
    {
        if (pos.inside(rect))
        {
            editing = true;
            topLeft->setVisible(true);
            return true;
        }

        // pos not in rect
        if (editing)
        {
            editing = false;
            return false;
        }
    }
    return false;
}

bool TextBox::mouseMoved(const Point &pos)
{
    return false;
}

bool TextBox::mouseReleased(const Point &pos)
{
    if (visible)
    {
        if (pos.inside(rect))
        {
            if (editing)
            {
                prevText = text;
                return true; // both click and release in the box
            }
        }
    }
    return false;
}

bool TextBox::keyPressed(int &key)
{
    if (locked) return true;
    bool keepFocus = true;
    if (key < 128 || key == 65288)
    {
        switch (key)
        {
        case -1:
            break;
        case 13: // \r
        case 10: // \n
            if (text.length() == 0)
            {
                text = prevText;
            }
            keepFocus = false;
            break;
        case 27: // ESC
            text = prevText;
            keepFocus = false;
            break;
        case 8: // BS
        case 65288: // BS on pi3 without QT over VNC
            if (text.length())
            {
                text.erase(text.length()-1);
            }
            break;
        default:
            text += (char)key;
            break;
        }
        recalcRect();
        key = -1; // consume key
    }
    return keepFocus;
}

void TextBox::lostFocus()
{
    if (editing)
    {
        editing = false;
    }
    topLeft->setVisible(false);
}

const string &TextBox::getCreateStatusMsg() const
{
   static const string msg = "";
   return msg;
}

const string &TextBox::getEditStatusMsg() const
{
   static const string msg = "Type lowercase text. ENTER to approve. ESC to cancel.";
   return msg;
}

void TextBox::recalcRect()
{
    baseline=0;
    Size textSize = getTextSize(text, fontFace,
                                fontScale, fontThickness, &baseline);
    baseline += thickness;
    rect = Rect((*topLeft)(),
                    (*topLeft)() + Point(textSize.width, textSize.height+baseline*2));
}

cv::Scalar TextBox::getFontColor() const
{
    return fontColor;
}

void TextBox::setFontColor(const cv::Scalar &value)
{
    fontColor = value;
    fontColor[3] = 255; // shape colors are opaque
}

int TextBox::getFontThickness() const
{
    return fontThickness;
}

void TextBox::setFontThickness(int value)
{
    fontThickness = value;
}

void TextBox::writeInternals(FileStorage &fs) const
{
    Shape::writeInternals(fs);
    fs << "text" << text;
    fs << "topLeft" << *topLeft;
    fs << "fontFace" << fontFace;
    fs << "fontScale" << fontScale;
    fs << "fontThickness" << fontThickness;
    fs << "fontColor" << fontColor;
}

void TextBox::readInternals(const FileNode &node)
{
    Shape::readInternals(node);
    node["text"] >> text;
    Shape *shape = 0;
    node["topLeft"] >> shape;
    topLeft.reset(dynamic_cast<Handle*>(shape));
    node["fontFace"] >> fontFace;
    node["fontScale"] >> fontScale;
    node["fontThickness"] >> fontThickness;
    node["fontColor"] >> fontColor;
    recalcRect();
    registerCBs();
}

void TextBox::registerCBs()
{
    topLeft->addPosChangedCB([this](const Point &)
    {
        recalcRect();
    });
}

list<Handle *> TextBox::getConnectionTargets()
{
    return {topLeft.get()};
}

shared_ptr<Shape> TextBox::getShape(int id)
{
   if (id == topLeft->getId())
   {
       return topLeft;
   }
   return nullptr;
}

const char *TextBox::getType() const
{
    return type;
}

const string &TextBox::getText() const
{
    return text;
}

void TextBox::setText(const string &value)
{
    text = value;
    recalcRect();
}

void TextBox::setTL(const Point &value)
{
    topLeft->setPos(value);
}

int TextBox::getFontFace() const
{
    return fontFace;
}

void TextBox::setFontFace(int value)
{
    fontFace = value;
    recalcRect();
}

double TextBox::getFontScale() const
{
    return fontScale;
}

void TextBox::setFontScale(double value)
{
    fontScale = value;
    recalcRect();
}

void TextBox::translate(const Point &offset)
{
   topLeft->translate(offset);
   recalcRect();
}

}
