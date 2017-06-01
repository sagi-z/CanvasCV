#ifndef BUTTON_H
#define BUTTON_H

#include <opencv2/opencv.hpp>
#include <list>
#include "colors.h"
#include "floatingtext.h"

namespace canvascv
{

class Button : public FloatingText
{
public:
    Button(cv::Point pos);

    Button(const std::string msgVal = "",
           cv::Point topLeftVal = cv::Point(),
           cv::Scalar colorVal = Colors::BLACK,
           cv::Scalar bgColorVal = Colors::P1_GRAY,
           double fontScaleVal = 0.5,
           int fontThicknessVal = 1,
           double alphaVal = 0.5,
           int fontFaceVal = cv::FONT_HERSHEY_COMPLEX_SMALL);

    virtual const char *getType() const;

    static std::shared_ptr<Button> newButton(Canvas &c,
                                             cv::Point pos,
                                             const std::string &buttonText,
                                             const std::string &statusMsg="");

    static const char *type;

protected:
    // TODO
    virtual void writeInternals(cv::FileStorage &fs) const
    {
    }
    // TODO
    virtual void readInternals(const cv::FileNode &node)
    {
    }

private:

    virtual void mousePressed();
    virtual void mouseReleased();
    virtual void mouseEnter();
    virtual void mouseLeave();

    double origAlpha;
};

}
#endif // BUTTON_H
