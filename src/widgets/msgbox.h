#ifndef MSGBOX_H
#define MSGBOX_H

#include "compoundwidget.h"

namespace canvascv
{

class VFrame;
class HorizontalLayout;
class FloatingText;
class Button;

class MsgBox : public CompoundWidget
{
public:

    MsgBox(const cv::Point &pos);

    virtual const char *getType() const;

    static std::shared_ptr<MsgBox> create(Layout &layout,
                                          const std::string &msg = "",
                                          const cv::Point &pos = cv::Point(0,0));

    void setMsg(const std::string &msg);
    void addButton(const std::string &buttonText, CBType cb);

    static const char *type;

private:
    VFrame *parts;
    HorizontalLayout * buttons;
};

}

#endif // MSGBOX_H
