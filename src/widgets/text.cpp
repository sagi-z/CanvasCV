#include "text.h"
#include "widgetfactory.h"
#include "layout.h"

using namespace cv;
using namespace std;

namespace canvascv
{

const char *Text::type = "Text";

Text::Text(const Point &pos)
    : Widget(pos),
      padding(4),
      doublePadding(padding*2),
      msg(),
      maxWidth(0),
      fontScale(0.5),
      fontFace(FONT_HERSHEY_COMPLEX_SMALL),
      fontHeight(0)
{
    setOutlineColor(Colors::Black);
    setFillColor(Colors::LightGray);
}

shared_ptr<Text> Text::create(Layout &layout,
                              const cv::Point &pos,
                              const string &text,
                              Anchor flowAnchor,
                              Anchor layoutAnchor)
{
    shared_ptr<Text> widget(WidgetFactoryT<Text>::newWidget(layout, pos));
    widget->setText(text);
    widget->setFlowAnchor(flowAnchor);
    widget->setLayoutAnchor(layoutAnchor);
    return widget;
}

shared_ptr<Text> Text::create(Layout &layout,
                              const string &text,
                              Anchor flowAnchor,
                              Anchor layoutAnchor)
{
    return create(layout, Point(0,0), text, flowAnchor, layoutAnchor);
}

const char *Text::getType() const
{
    return type;
}

string Text::getText() const
{
    return msg;
}

void Text::setText(const string &value)
{
    if (msg != value)
    {
        msg = value;
        setDirty();
    }
}

int Text::getFontFace() const
{
    return fontFace;
}

void Text::setFontFace(int value)
{
    if (fontFace != value)
    {
        fontFace = value;
        setDirty();
    }
}

double Text::getFontScale() const
{
    return fontScale;
}

void Text::setFontScale(double value)
{
    if (fontScale != value)
    {
        fontScale = value;
        setDirty();
    }
}

void Text::drawFG(Mat &dst)
{
    if (rows.size())
    {
        int yEnd = dst.rows;
        int y = fontHeight;
        for (auto &strRow : rows)
        {
            Point textPos(padding, y); // aligh to left by default
            if (flowAnchor & CENTER)
            {
                textPos.x += (dst.cols - strRow.width) / 2.;
            }
            else if (flowAnchor & RIGHT)
            {
                textPos.x = location.x + dst.cols - padding - strRow.width;
            }
            putText(dst, strRow.str, textPos,
                    fontFace, fontScale, getOutlineColor(), thickness, LINE_AA);
            y += fontHeight;
            if (y > yEnd) break;
        }
    }
}

void Text::prepareMsgParts()
{
    rows.clear();
    if (msg.length())
    {
        if (! layout) return;
        int localMaxWidth = maxWidth;
        if (localMaxWidth < doublePadding) localMaxWidth = doublePadding;

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
            int width = doublePadding + textSize.width; // padding pixels at start & end = doublePadding
            fontHeight = textSize.height + baseline + padding;
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
                rectWidth = min(localMaxWidth - padding, // "absolute limit width" vs.
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
                if (lineData.width   // has +doublePadding pixels for padding
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

int Text::getPadding() const
{
    return padding;
}

void Text::setPadding(int value)
{
    if (padding != value)
    {
        padding = value;
        doublePadding = padding * 2;
        setDirty();
    }
}

int Text::getMaxWidth() const
{
    return maxWidth;
}

void Text::setMaxWidth(int value)
{
    if (maxWidth != value)
    {
        maxWidth = value;
        setDirty();
    }
}

int Text::getFontHeight() const
{
    return fontHeight;
}

const Rect &Text::getRect()
{
    return rect;
}

const Rect &Text::getMinimalRect()
{
    return minimalRect;
}

void Text::recalc()
{
    prepareMsgParts();
    callDrawFG();
}

}
