#include "floatingtext.h"
#include "widgetfactory.h"
#include "layout.h"

using namespace cv;
using namespace std;

namespace canvascv
{

const char *FloatingText::type = "FloatingText";

FloatingText::FloatingText(Layout &layoutVal, const Point &pos)
    : Widget(layoutVal, pos),
      msg(),
      maxWidth(0),
      fontScale(0.5),
      fontFace(FONT_HERSHEY_COMPLEX_SMALL),
      fontHeight(0)
{
    setOutlineColor(Colors::Black);
    setFillColor(Colors::LightGray);
}

FloatingText::FloatingText(Layout &layoutVal, const string msgVal, Point locationVal, int maxWidthVal, Scalar colorVal,
                           Scalar bgColorVal, double fontScaleVal, int thicknessVal, int fontFaceVal)
    : Widget(layoutVal, locationVal),
      msg(msgVal),
      maxWidth(maxWidthVal),
      fontScale(fontScaleVal),
      fontFace(fontFaceVal),
      fontHeight(0)
{
    setOutlineColor(colorVal);
    setFillColor(bgColorVal);
    thickness = thicknessVal;
}

shared_ptr<FloatingText> FloatingText::create(Layout &layout,
                                              const cv::Point &pos,
                                              const string &text,
                                              Anchor flowAnchor,
                                              Anchor layoutAnchor)
{
    shared_ptr<FloatingText> widget(WidgetFactoryT<FloatingText>::newWidget(layout, pos));
    widget->setMsg(text);
    widget->setFlowAnchor(flowAnchor);
    widget->setLayoutAnchor(layoutAnchor);
    return widget;
}

shared_ptr<FloatingText> FloatingText::create(Layout &layout,
                                              const string &text,
                                              Anchor flowAnchor,
                                              Anchor layoutAnchor)
{
    return create(layout, Point(0,0), text, flowAnchor, layoutAnchor);
}

const char *FloatingText::getType() const
{
   return type;
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

void FloatingText::drawFG(Mat &dst)
{
    if (rows.size())
    {
        int yEnd = dst.rows;
        int y = fontHeight;
        for (auto &strRow : rows)
        {
            Point textPos(5, y); // aligh to left by default
            if (flowAnchor & CENTER)
            {
                textPos.x += (dst.cols - strRow.width) / 2.;
            }
            else if (flowAnchor & RIGHT)
            {
                textPos.x = location.x + dst.cols - 5 - strRow.width;
            }
            putText(dst, strRow.str, textPos,
                    fontFace, fontScale, getOutlineColor(), thickness, LINE_AA);
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
        int localMaxWidth = maxWidth;
        if (localMaxWidth < 10) localMaxWidth = 10;

        std::list<StringRow> msgParts;
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
            if (maxWidth)
            {
                totalRows += width / localMaxWidth + 1;
            }
            else
            {
                ++totalRows;
            }
            maxNeededWidth = max(maxNeededWidth, width);
            msgParts.push_back({line, width});
        }
        if (totalRows)
        {
            int yRectStart;
            if (flowAnchor & BOTTOM)
            {
                yRectStart = (location.y - fontHeight * totalRows) - fontHeight;
                if (yRectStart < 0) yRectStart = 0;
            }
            else // TOP
            {
                yRectStart = location.y;
            }
            int rectHeight = fontHeight * totalRows + fontHeight;
            int rectWidth = maxNeededWidth;
            if (maxWidth)
            {
                rectWidth = min(localMaxWidth - 5, // "absolute limit width" vs.
                                maxNeededWidth);   // "width which is realy needed"
            }
            minimalRect = Rect(location.x, yRectStart, rectWidth, rectHeight);
            if (forcedWidth > rectWidth) rectWidth = forcedWidth;
            if (forcedHeight > rectHeight) rectHeight = forcedHeight;
            rect = Rect(location.x, yRectStart, rectWidth, rectHeight);
            for (StringRow &lineData : msgParts)
            {
                int numRows=1;
                double ratio = 1.;
                int realWidth = lineData.width;
                if (lineData.width   // has +10 pixels for padding
                        > rectWidth) // actual boundaries
                {
                    // wrap a long line to numRows lines
                    numRows = lineData.width / rectWidth + 1;
                    ratio = (double) lineData.width / rectWidth;
                    realWidth = rectWidth;
                }
                int stringPartsLen = floor(lineData.str.length() / ratio);
                for (int i = 0; i < numRows; ++i)
                {
                    int start = i * stringPartsLen;
                    int left = lineData.str.length() - start;
                    int len = left > stringPartsLen ? stringPartsLen : left;
                    rows.push_back({lineData.str.substr(start, len), realWidth});
                }
            }
        }
    }
    else
    {
        rect = minimalRect = Rect();
    }
    allocateBG(rect.size());
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

void FloatingText::recalc()
{
    prepareMsgParts();
    callDrawFG();
}

}
