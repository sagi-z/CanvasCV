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
      fontScale(0.5),
      fontThickness(1),
      fontFace(FONT_HERSHEY_COMPLEX_SMALL),
      fontHeight(0),
      flowDirection(TOP_DOWN)
{
    outlineColor = Colors::BLACK;
    fillColor = Colors::P1_GRAY;
}

FloatingText::FloatingText(const string msgVal, Point leftPosVal, Scalar colorVal,
                           Scalar bgColorVal, double fontScaleVal, int fontThicknessVal,
                           double alphaVal, int fontFaceVal)
    : Widget(),
      alpha(alphaVal),
      msg(msgVal),
      leftPos(leftPosVal),
      fontScale(fontScaleVal),
      fontThickness(fontThicknessVal),
      fontFace(fontFaceVal),
      fontHeight(0),
      flowDirection(TOP_DOWN)
{
    outlineColor = colorVal;
    fillColor = bgColorVal;
    prepareMsgParts();
}

std::shared_ptr<FloatingText> FloatingText::newFloatingText(Canvas &c, Point pos,
                                                            const string &text,
                                                            FloatingText::FlowDirection flow)
{

    shared_ptr<FloatingText> widget = c.createWidget<FloatingText>(pos);
    widget->setMsg(text);
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
    prepareMsgParts();
}

int FloatingText::getFontFace() const
{
    return fontFace;
}

void FloatingText::setFontFace(int value)
{
    fontFace = value;
    prepareMsgParts();
}

double FloatingText::getFontScale() const
{
    return fontScale;
}

void FloatingText::setFontScale(double value)
{
    fontScale = value;
    prepareMsgParts();
}

int FloatingText::getFontThickness() const
{
    return fontThickness;
}

void FloatingText::setFontThickness(int value)
{
    fontThickness = value;
    prepareMsgParts();
}

void FloatingText::draw(Mat &dst)
{
    int totalRows=0;
    int rectWidth = 0;
    for (LineData &lineData : msgParts)
    {
        totalRows += lineData.width / dst.cols + 1;
        rectWidth = max(rectWidth, lineData.width);
    }
    if (totalRows)
    {
        int yStart, yRectStart;
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
        int rectHeight = min((int) floor(fontHeight * totalRows + fontHeight), dst.rows - yRectStart - 1);
        rectWidth = min(dst.cols - leftPos.x - 10, rectWidth);
        rect = Rect(leftPos.x, yRectStart, rectWidth, rectHeight);
        int yRectEnd = yRectStart + rectHeight;
        Mat roi = dst(rect);
        Mat rectColor(roi.size(), CV_8UC3, fillColor);
        cv::addWeighted(rectColor, alpha, roi, 1.0 - alpha , 0.0, roi);
        cv::rectangle(dst, rect, fillColor);
        for (LineData &lineData : msgParts)
        {
            int numRows=1;
            double ratio = 1.;
            if (lineData.width > rectWidth)
            {
                // wrap a long line to numRows lines
                numRows = lineData.width / rectWidth + 1;
                ratio = (double) lineData.width / rectWidth;
            }
            int stringPartsLen = floor(lineData.str.length() / ratio);
            int y = yStart;
            for (int i = 0;
                 i < numRows && y < yRectEnd;
                 ++i, y += fontHeight)
            {
                int start = i * stringPartsLen;
                int left = lineData.str.length() - start;
                int len = left > stringPartsLen ? stringPartsLen : left;
                Point textPos(leftPos.x + 5, y);
                putText(dst, lineData.str.substr(start, len), textPos,
                        fontFace, fontScale, outlineColor, fontThickness, LINE_AA);
            }
            yStart = y;
        }
    }
}

void FloatingText::prepareMsgParts()
{
    msgParts.clear();
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
        msgParts.push_back({line, width});
    }
}

FloatingText::FlowDirection FloatingText::getFlowDirection() const
{
    return flowDirection;
}

void FloatingText::setFlowDirection(const FlowDirection &value)
{
    flowDirection = value;
}

cv::Point FloatingText::getLeftPos() const
{
    return leftPos;
}

void FloatingText::setLeftPos(const cv::Point &value)
{
    leftPos = value;
}

int FloatingText::getFontHeight() const
{
    return fontHeight;
}

}
