#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "handle.h"
#include "shape.h"
#include "shapefactory.h"

#include <string>
#include <memory>

namespace canvasvc
{

class TextBox : public Shape
{
public:
    TextBox(const cv::Point &pos);

    virtual void draw(cv::Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false);
    virtual bool mouseMoved(const cv::Point &pos);
    virtual bool mouseReleased(const cv::Point &pos);
    virtual bool isAtPos(const cv::Point &pos)
    {
        return rect.contains(pos);
    }

    virtual std::list<Handle *> getConnectionTargets();
    virtual std::shared_ptr<Shape> getShape(int id);
    virtual const char *getType() const
    {
        return "TextBox";
    }

    const std::string & getText() const
    {
        return text;
    }

    void setText(const std::string &value)
    {
        text = value;
        recalcRect();
    }

    void setTL(const cv::Point &value)
    {
        topLeft->setPos(value);
    }

    virtual bool keyPressed(int &key);
    virtual void lostFocus();

    int getFontFace() const
    {
        return fontFace;
    }

    void setFontFace(int value)
    {
        fontFace = value;
    }

    double getFontScale() const
    {
        return fontScale;
    }

    void setFontScale(double value)
    {
        fontScale = value;
    }

protected:
    virtual void writeInternals(cv::FileStorage &fs) const;
    virtual void readInternals(const cv::FileNode &node);
    void registerCBs();

private:
    void recalcRect()
    {
        baseline=0;
        Size textSize = getTextSize(text, fontFace,
                                    fontScale, thickness, &baseline);
        baseline += thickness;
        rect = cv::Rect((*topLeft)(),
                        (*topLeft)() + cv::Point(textSize.width, +textSize.height+baseline*2));
    }

    std::string text;
    std::string prevText;
    int fontFace;
    double fontScale;
    int baseline;
    std::shared_ptr<Handle> topLeft;
    cv::Rect rect;
    cv::Point dragPos;
};

}

#endif // TEXTBOX_H
