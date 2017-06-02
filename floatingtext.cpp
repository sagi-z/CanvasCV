#include "floatingtext.h"
#include "canvas.h"

using namespace cv;
using namespace std;

namespace canvascv
{

const char *FloatingText::type = "FloatingText";

FloatingText::FloatingText(Point pos)
    : Widget(),
      alpha(0.5),
      msg(),
      leftPos(pos),
      maxWidth(0),
      fontScale(0.5),
      fontThickness(1),
      fontFace(FONT_HERSHEY_COMPLEX_SMALL),
      fontHeight(0),
      flowDirection(TOP_DOWN)
{
    outlineColor = Colors::BLACK;
    fillColor = Colors::P1_GRAY;
}

FloatingText::FloatingText(const string msgVal, Point leftPosVal, int maxWidthVal, Scalar colorVal,
                           Scalar bgColorVal, double fontScaleVal, int fontThicknessVal,
                           double alphaVal, int fontFaceVal)
    : Widget(),
      alpha(alphaVal),
      msg(msgVal),
      leftPos(leftPosVal),
      maxWidth(maxWidthVal),
      fontScale(fontScaleVal),
      fontThickness(fontThicknessVal),
      fontFace(fontFaceVal),
      fontHeight(0),
      flowDirection(TOP_DOWN)
{
    outlineColor = colorVal;
    fillColor = bgColorVal;
}

std::shared_ptr<FloatingText> FloatingText::newFloatingText(Canvas &c, Point pos,
                                                            const string &text,
                                                            int maxWidthVal,
                                                            FloatingText::FlowDirection flow)
{

    shared_ptr<FloatingText> widget = c.createWidget<FloatingText>(pos);
    widget->setMsg(text);
    widget->setMaxWidth(maxWidthVal);
    widget->setFlowDirection(flow);
    return widget;
}

bool FloatingText::isAtPos(const Point &pos)
{
    if (rect.contains(pos))
    {
        return true;
    }
    return false;
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
    msg = value;
    rows.clear();
}

int FloatingText::getFontFace() const
{
    return fontFace;
}

void FloatingText::setFontFace(int value)
{
    fontFace = value;
    rows.clear();
}

double FloatingText::getFontScale() const
{
    return fontScale;
}

void FloatingText::setFontScale(double value)
{
    fontScale = value;
    rows.clear();
}

int FloatingText::getFontThickness() const
{
    return fontThickness;
}

void FloatingText::setFontThickness(int value)
{
    fontThickness = value;
    rows.clear();
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
        for (auto &str : rows)
        {
            Point textPos(leftPos.x + 5, y);
            putText(dst, str, textPos,
                    fontFace, fontScale, outlineColor, fontThickness, LINE_AA);
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
        if (! canvas) return;
        Size canvasSize = canvas->getSize();
        int localMaxWidth = canvasSize.width - leftPos.x;
        if (maxWidth && maxWidth < localMaxWidth) localMaxWidth = maxWidth;
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
                                        fontScale, fontThickness,
                                        &baseline);
            baseline += fontThickness;
            int width = 10 + textSize.width; // 5 pixels at start & end = 10
            fontHeight = textSize.height+baseline*2;
            totalRows += width / localMaxWidth + 1;
            maxNeededWidth = max(maxNeededWidth, width);
            msgParts.push_back({line, width});
        }
        if (totalRows)
        {
            int yRectStart;
            if (flowDirection == TOP_DOWN)
            {
                yStart = leftPos.y + fontHeight;
                if (yStart < 0) yStart = 0;
                yRectStart = leftPos.y;
            }
            else
            {
                yStart = leftPos.y - fontHeight * totalRows;
                if (yStart < 0) yStart = 0;
                yRectStart = yStart - fontHeight;
            }
            int rectHeight = min((int) floor(fontHeight * totalRows + fontHeight),
                                 canvasSize.height - yRectStart - 1);
            int rectWidth = min(localMaxWidth - 5, // "absolute limit width (5 pixels from right of canvas)" vs.
                                maxNeededWidth);   // "width which is realy needed"
            rect = Rect(leftPos.x, yRectStart, rectWidth, rectHeight);
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
    maxWidth = value;
    rows.clear();
}

FloatingText::FlowDirection FloatingText::getFlowDirection() const
{
    return flowDirection;
}

void FloatingText::setFlowDirection(const FlowDirection &value)
{
    flowDirection = value;
    rows.clear();
}

void FloatingText::canvasResized(const Size &size)
{
    rows.clear();
}

cv::Point FloatingText::getLeftPos() const
{
    return leftPos;
}

void FloatingText::setLeftPos(const cv::Point &value)
{
    leftPos = value;
    rows.clear();
}

int FloatingText::getFontHeight() const
{
    return fontHeight;
}

}
