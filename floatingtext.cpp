#include "floatingtext.h"

using namespace cv;
using namespace std;

namespace canvascv
{

FloatingText::FloatingText(const string msgVal, Point topLeftVal, Scalar colorVal,
                           Scalar bgColorVal, double fontScaleVal, int fontThicknessVal,
                           double alphaVal, int fontFaceVal)
    : msg(msgVal),
      topLeft(topLeftVal),
      color(colorVal),
      bgColor(bgColorVal),
      fontScale(fontScaleVal),
      fontThickness(fontThicknessVal),
      alpha(alphaVal),
      fontFace(fontFaceVal),
      fontHeight(0)
{
    prepareMsgParts();
}

double FloatingText::getAlpha() const
{
    return alpha;
}

void FloatingText::setAlpha(double value)
{
    alpha = value;
}

Scalar FloatingText::getColor() const
{
    return color;
}

void FloatingText::setColor(const Scalar &value)
{
    color = value;
}

Scalar FloatingText::getBgColor() const
{
    return bgColor;
}

void FloatingText::setBgColor(const Scalar &value)
{
    bgColor = value;
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
        int yStart = topLeft.y + fontHeight;
        rectWidth = min(dst.cols - topLeft.x - 10, rectWidth);
        int rectHeight = min((int) floor(fontHeight * totalRows + fontHeight), dst.rows - (yStart - fontHeight) - 1);
        Rect rect = Rect(topLeft.x, topLeft.y, rectWidth, rectHeight);
        Mat roi = dst(rect);
        Mat rectColor(roi.size(), CV_8UC3, bgColor);
        cv::addWeighted(rectColor, alpha, roi, 1.0 - alpha , 0.0, roi);
        cv::rectangle(dst, rect, bgColor);
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
                 i < numRows;
                 ++i, y += fontHeight)
            {
                int start = i * stringPartsLen;
                int left = lineData.str.length() - start;
                int len = left > stringPartsLen ? stringPartsLen : left;
                putText(dst, lineData.str.substr(start, len), Point(topLeft.x+5, y),
                        fontFace, fontScale, color, fontThickness, LINE_AA);
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

int FloatingText::getFontHeight() const
{
    return fontHeight;
}

cv::Point FloatingText::getTopLeft() const
{
    return topLeft;
}

void FloatingText::setTopLeft(const cv::Point &value)
{
    topLeft = value;
}

}
