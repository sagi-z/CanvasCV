#ifndef LINECROSSING_H
#define LINECROSSING_H

#include "arrow.h"
#include "line.h"
#include "textbox.h"
#include "shapefactory.h"
#include <memory>

namespace canvascv
{

class LineCrossing : public CompoundShape
{
public:
    LineCrossing(const cv::Point& pos);

    virtual bool isAtPos(const cv::Point &pos)
    {
        return line->isAtPos(pos);
    }

    virtual std::list<Handle *> getConnectionTargets();
    virtual const char *getType() const;

    int getDirection() const;

    const std::string &getName() const;

    void setName(const std::string &value) const;

    int getArrowMagnitude() const;

    void setArrowMagnitude(int mag);

    const cv::Point &getTail() const;

    const cv::Point &getHead() const;

    /// getter to internal TextBox
    TextBox *getTextBox();

    /// getter to internal Line
    Line *getLine();

    /// getter to internal Arrow
    Arrow *getArrow();

    static const char * type;

protected:
    virtual void draw(cv::Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false);

    void registerCBs();

    void recalcArrow();

    void recalcTextBox();

    cv::Point middlePoint();

    bool isMiddlePoint(const cv::Point &pos, int threshold=3);

    void arrowTailHead(cv::Point &tail, cv::Point &head);

    virtual void writeInternals(cv::FileStorage &fs) const;
    virtual void readInternals(const cv::FileNode &node);

    virtual void reloadPointers(const std::list<Shape*> &lst, std::list<Shape*>::const_iterator &i);

    virtual const std::string &getStatusMsg() const;

private:
    Line* line;
    Arrow* arrow;
    TextBox* textBox;
    int direction;
    int arrowMagnitude;
};

}

#endif // LINECROSSING_H
