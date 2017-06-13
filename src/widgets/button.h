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
    Button(const cv::Point &pos);

    Button(const std::string msgVal = "",
           cv::Point locationVal = cv::Point(0, 0),
           int maxWidthVal = 0,
           cv::Scalar colorVal = Colors::BLACK,
           cv::Scalar bgColorVal = Colors::P1_GRAY,
           double fontScaleVal = 0.5,
           int fontThicknessVal = 1,
           int fontFaceVal = cv::FONT_HERSHEY_COMPLEX_SMALL);

    virtual const char *getType() const;

    static std::shared_ptr<Button> create(Layout &layout,
                                          const cv::Point &pos,
                                          const std::string &buttonText,
                                          const std::string &statusMsg="",
                                          int maxWidthVal = 0);

    static std::shared_ptr<Button> create(Layout &layout,
                                          const std::string &buttonText,
                                          const std::string &statusMsg="",
                                          int maxWidthVal = 0);

    static const char *type;

protected:
    virtual void recalc();

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
};

}
#endif // BUTTON_H
