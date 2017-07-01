#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "handle.h"
#include "shape.h"
#include "shapefactory.h"

#include <string>
#include <memory>

namespace canvascv
{

/**
 * @brief The TextBox class
 * 
 * Allows you to draw a line by mouse or from code
 */
class TextBox : public Shape
{
public:
    /// get the displayed text
    const std::string & getText() const;

    /// set the displayed text
    void setText(const std::string &value);

    /// set position (top left corner)
    void setTL(const cv::Point &value);

    /// get the font used
    int getFontFace() const;

    /// set the font used
    void setFontFace(int value);

    /// get the scale of the used font
    double getFontScale() const;

    /// set the scale of the used font
    void setFontScale(double value);

    /// get the thickness of the font
    int getFontThickness() const;

    /// set the thickness of the font
    void setFontThickness(int value);

    /// get the font color
    cv::Scalar getFontColor() const;

    /// set the font color
    void setFontColor(const cv::Scalar &value);

    virtual bool isAtPos(const cv::Point &pos)
    {
        return rect.contains(pos);
    }

    virtual std::list<Handle *> getConnectionTargets();
    virtual std::shared_ptr<Shape> getShape(int id);

    virtual void translate(const cv::Point &offset);

    virtual const char *getType() const;

    static const char * type;

protected:
    friend class ShapeFactory;
    template <class T> friend class ShapeFactoryT;

    TextBox(const cv::Point &pos);

    virtual void writeInternals(cv::FileStorage &fs) const;
    virtual void readInternals(const cv::FileNode &node);
    void registerCBs();

    virtual void draw(cv::Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false);
    virtual bool mouseMoved(const cv::Point &pos);
    virtual bool mouseReleased(const cv::Point &pos);

    virtual bool keyPressed(int &key);
    virtual void lostFocus();

    virtual const string &getCreateStatusMsg() const;
    virtual const string &getEditStatusMsg() const;
private:
    void recalcRect();

    std::string text;
    std::string prevText;
    int fontFace;
    double fontScale;
    int fontThickness;
    cv::Scalar fontColor;
    int baseline;
    std::shared_ptr<Handle> topLeft;
    cv::Rect rect;
};

}

/** @example example_shapes_widgets.cpp
 * This is an example of how to create shapes by code and mouse.
 */

#endif // TEXTBOX_H
