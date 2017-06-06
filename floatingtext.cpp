#include "floatingtext.h"
#include "canvas.h"

using namespace cv;
using namespace std;

namespace canvascv
{

const char *FloatingText::type = "FloatingText";

FloatingText::FloatingText(const Point &pos)
    : Widget(pos),
      alpha(0.5),
      msg(),
      maxWidth(0),
      fontScale(0.5),
      fontFace(FONT_HERSHEY_COMPLEX_SMALL),
      fontHeight(0)
{
    outlineColor = Colors::BLACK;
    fillColor = Colors::P1_GRAY;
}

FloatingText::FloatingText(const string msgVal, Point locationVal, int maxWidthVal, Scalar colorVal,
                           Scalar bgColorVal, double fontScaleVal, int thicknessVal,
                           double alphaVal, int fontFaceVal)
    : Widget(locationVal),
      alpha(alphaVal),
      msg(msgVal),
      maxWidth(maxWidthVal),
      fontScale(fontScaleVal),
      fontFace(fontFaceVal),
      fontHeight(0)
{
    outlineColor = colorVal;
    fillColor = bgColorVal;
    thickness = thicknessVal;
}

shared_ptr<FloatingText> FloatingText::newFloatingText(Canvas &c, Point pos,
                                                       const string &text,
                                                       Widget::Anchor anchor)
{

    shared_ptr<FloatingText> widget = c.createWidget<FloatingText>(pos);
    widget->setMsg(text);
    widget->setAnchor(anchor);
    return widget;
}

const char *FloatingText::getType() const
{
   return type;
}

double FloatingText::getAlpha() const
{
    return alpha;
}

void FloatingText::setAlpha(double value)
{
    alpha = value;
}

string FloatingText::getMsg() const
{
    return msg;
}

void FloatingText::setMsg(const string &value)
{
    if (msg != value)
    {
        msg = value;
        setDirty();
    }
}

int FloatingText::getFontFace() const
{
    return fontFace;
}

void FloatingText::setFontFace(int value)
{
    if (fontFace != value)
    {
        fontFace = value;
        setDirty();
    }
}

double FloatingText::getFontScale() const
{
    return fontScale;
}

void FloatingText::setFontScale(double value)
{
    if (fontScale != value)
    {
        fontScale = value;
        setDirty();
    }
}

void FloatingText::draw(Mat &dst)
{
    if (msg.length())
    {
        if (! rows.size())
            prepareMsgParts();

        int yEnd = rect.tl().y + rect.height;
        Mat roi = dst(rect);
        cv::addWeighted(rectColor, alpha, roi, 1.0 - alpha , 0.0, roi);
        cv::rectangle(dst, rect, fillColor);
        int y = yStart;
        int padding = (rect.width - minimalRect.width) / 2.;
        for (auto &str : rows)
        {
            Point textPos(location.x + 5, y);
            if (padding > 0)
            {  // FIXME: centering the text as a whole and not per line
                textPos.x += padding;
            }
            putText(dst, str, textPos,
                    fontFace, fontScale, outlineColor, thickness, LINE_AA);
            y += fontHeight;
            if (y > yEnd) break;
        }
    }
}

void FloatingText::prepareMsgParts()
{
    rows.clear();
    if (msg.length())
    {
        if (! layout) return;
        Size layoutSize = layout->getAllowedSize();
        int localMaxWidth = layoutSize.width - location.x;
        if (maxWidth && maxWidth < localMaxWidth) localMaxWidth = maxWidth;
        if (localMaxWidth < 10) localMaxWidth = 10;
        struct LineData
        {
            std::string str;
            int width;
        };

        std::list<LineData> msgParts;
        int totalRows=0;
        int maxNeededWidth = 0;
        int pos = 0;
        int prevPos = 0;
        while (pos < msg.length())
        {
            while(pos++ < msg.length() && msg[pos-1] != '\n');
            string line(msg, prevPos, pos - prevPos - 1);
            prevPos = pos;
            int baseline=0;
            Size textSize = getTextSize(line, fontFace,
                                        fontScale, thickness,
                                        &baseline);
            baseline += thickness;
            int width = 10 + textSize.width; // 5 pixels at start & end = 10
            fontHeight = textSize.height+baseline*2;
            totalRows += width / localMaxWidth + 1;
            maxNeededWidth = max(maxNeededWidth, width);
            msgParts.push_back({line, width});
        }
        if (totalRows)
        {
            int yRectStart;
            if (anchor == TOP_LEFT)
            {
                yStart = location.y + fontHeight;
                if (yStart < 0) yStart = 0;
                yRectStart = location.y;
            }
            else if (anchor == BOTTOM_LEFT)
            {
                yStart = location.y - fontHeight * totalRows;
                if (yStart < 0) yStart = 0;
                yRectStart = yStart - fontHeight;
            }
            else
            {
                cerr << "anchor type not supported" << endl;
                abort();
            }
            int rectHeight = min((int) floor(fontHeight * totalRows + fontHeight),
                                 layoutSize.height - yRectStart - 1);
            int rectWidth = min(localMaxWidth - 5, // "absolute limit width (5 pixels from right of layout)" vs.
                                maxNeededWidth);   // "width which is realy needed"
            minimalRect = Rect(location.x, yRectStart, rectWidth, rectHeight);
            if (forcedWidth > rectWidth) rectWidth = forcedWidth;
            if (forcedHeight > rectHeight) rectHeight = forcedHeight;
            rect = Rect(location.x, yRectStart, rectWidth, rectHeight);
            rectColor = Mat(rect.size(), CV_8UC3, fillColor);
            for (LineData &lineData : msgParts)
            {
                int numRows=1;
                double ratio = 1.;
                if (lineData.width   // has +10 pixels for padding
                        > rectWidth) // actual boundaries
                {
                    // wrap a long line to numRows lines
                    numRows = lineData.width / rectWidth + 1;
                    ratio = (double) lineData.width / rectWidth;
                }
                int stringPartsLen = floor(lineData.str.length() / ratio);
                for (int i = 0; i < numRows; ++i)
                {
                    int start = i * stringPartsLen;
                    int left = lineData.str.length() - start;
                    int len = left > stringPartsLen ? stringPartsLen : left;
                    rows.push_back(lineData.str.substr(start, len));
                }
            }
        }
    }
}

int FloatingText::getMaxWidth() const
{
    return maxWidth;
}

void FloatingText::setMaxWidth(int value)
{
    if (maxWidth != value)
    {
        maxWidth = value;
        setDirty();
    }
}

void FloatingText::layoutResized(const Size &size)
{
    setDirty();
}

int FloatingText::getFontHeight() const
{
    return fontHeight;
}

const Rect &FloatingText::getRect()
{
    return rect;
}

const Rect &FloatingText::getMinimalRect()
{
    return minimalRect;
}

void FloatingText::translate(const Point &translation)
{
    if (translation.x != 0 || translation.y != 0)
    {
        Widget::translate(translation);
        setDirty();
    }
}

void FloatingText::recalc()
{
    prepareMsgParts();
}

}
