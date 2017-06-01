#include "textbox.h"
#include "colors.h"

#include <opencv2/imgproc.hpp>

namespace canvascv
{

const char * TextBox::type = "TextBox";

TextBox::TextBox(const cv::Point &pos) :
    Shape(),
    text("text"),
    prevText(text),
    fontFace(FONT_HERSHEY_PLAIN),
    fontScale(1)
{
    topLeft.reset(dynamic_cast<Handle*>(ShapeFactoryT<Handle>::newShape(pos)));
    topLeft->setLocked(true);
    recalcRect();
    registerCBs();
}


void TextBox::draw(cv::Mat &canvas)
{
    if (visible)
    {
        if (editing)
        {
            cv::Rect rectSelected(rect);
            rectSelected.x -= 2;
            rectSelected.y -= 2;
            rectSelected.width += 4;
            rectSelected.height += 4;
            rectangle(canvas, rectSelected, outlineColor, thickness);
        }
        rectangle(canvas, rect, outlineColor, thickness);
        putText(canvas, text, cv::Point(rect.tl().x,rect.tl().y+baseline*2), fontFace, fontScale,
                outlineColor, thickness, LINE_AA);
        topLeft->draw(canvas);
    }
}

bool TextBox::mousePressed(const cv::Point &pos, bool onCreate)
{
    if (visible)
    {
        if (! onCreate && ! locked)
        {
            dragPos = pos;
        }
        if (pos.inside(rect))
        {
            editing = true;
            topLeft->setVisible(true);
            return true;
        }

        // pos not in cv::rect
        if (editing)
        {
            editing = false;
            return false;
        }
    }
    return false;
}

bool TextBox::mouseMoved(const cv::Point &pos)
{
    if (dragPos.x || dragPos.y)
    {
        cv::Point diff = dragPos - pos;
        setTL((*topLeft)() - diff);
        dragPos = pos;
        return true;
    }
    return false;
}

bool TextBox::mouseReleased(const cv::Point &pos)
{
    if (visible)
    {
        if (dragPos.x || dragPos.y)
        {
            dragPos.x = 0;
            dragPos.y = 0;
            return true;
        }

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

void TextBox::writeInternals(cv::FileStorage &fs) const
{
    Shape::writeInternals(fs);
    fs << "text" << text;
    fs << "topLeft" << *topLeft;
    fs << "fontFace" << fontFace;
    fs << "fontScale" << fontScale;
}

void TextBox::readInternals(const cv::FileNode &node)
{
    Shape::readInternals(node);
    node["text"] >> text;
    Shape *shape = 0;
    node["topLeft"] >> shape;
    topLeft.reset(dynamic_cast<Handle*>(shape));
    node["fontFace"] >> fontFace;
    node["fontScale"] >> fontScale;
    recalcRect();
    registerCBs();
}

void TextBox::registerCBs()
{
    topLeft->addPosChangedCB([this](const cv::Point &)
    {
        recalcRect();
    });
}

std::list<Handle *> TextBox::getConnectionTargets()
{
    return {topLeft.get()};
}

std::shared_ptr<Shape> TextBox::getShape(int id)
{
   if (id == topLeft->getId())
   {
       return topLeft;
   }
   return nullptr;
}

}
