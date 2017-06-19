#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "handle.h"
#include "shape.h"
#include "shapefactory.h"

#include <string>
#include <memory>

namespace canvascv
{

class TextBox : public Shape
{
public:
    TextBox(const cv::Point &pos);

    virtual bool isAtPos(const cv::Point &pos)
    {
        return rect.contains(pos);
    }

    virtual std::list<Handle *> getConnectionTargets();
    virtual std::shared_ptr<Shape> getShape(int id);

    virtual const char *getType() const;

    const std::string & getText() const;

    void setText(const std::string &value);

    void setTL(const cv::Point &value);

    int getFontFace() const;

    void setFontFace(int value);

    double getFontScale() const;

    void setFontScale(double value);

    virtual void translate(const cv::Point &offset);

    static const char * type;

    int getFontThickness() const;
    void setFontThickness(int value);

protected:
    virtual void writeInternals(cv::FileStorage &fs) const;
    virtual void readInternals(const cv::FileNode &node);
    void registerCBs();

    virtual void draw(cv::Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false);
    virtual bool mouseMoved(const cv::Point &pos);
    virtual bool mouseReleased(const cv::Point &pos);

    virtual bool keyPressed(int &key);
    virtual void lostFocus();

private:
    void recalcRect();

    std::string text;
    std::string prevText;
    int fontFace;
    double fontScale;
    int fontThickness;
    int baseline;
    std::shared_ptr<Handle> topLeft;
    cv::Rect rect;
};

}

#endif // TEXTBOX_H
