#ifndef MSGBOX_H
#define MSGBOX_H

#include "compoundwidget.h"
#include "vframe.h"
#include "horizontallayout.h"

namespace canvascv
{

class VFrame;
class HorizontalLayout;

class MsgBox : public CompoundWidget
{
public:

    virtual const char *getType() const;

    static std::shared_ptr<MsgBox> create(Layout &layout,
                                          const std::string &msg = "",
                                          const cv::Point &pos = cv::Point(0,0));

    void setMsg(const std::string &msg);
    void addButton(const std::string &buttonText, CBType cb);

    static const char *type;

protected:
    friend class WidgetFactory;
    template <class T> friend class WidgetFactoryT;

    MsgBox(Layout &layoutVal, const cv::Point &pos);

private:
    std::shared_ptr<VFrame> parts;
    std::shared_ptr<HorizontalLayout> buttons;
};

}

#endif // MSGBOX_H
