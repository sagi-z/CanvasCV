#ifndef LINECROSSING_H
#define LINECROSSING_H

#include "arrow.h"
#include "line.h"
#include "textbox.h"
#include "shapefactory.h"
#include <memory>

namespace canvasvc
{

class LineCrossing : public CompoundShape
{
public:
    LineCrossing(const cv::Point& pos)
        : direction(1)
    {
        line = addShape<Line>(pos);
        line->setOutlineColor(Colors::P1_LBLUE);
        line->setThickness(3);
        arrow = addShape<Arrow>(pos);
        arrow->setOutlineColor(Colors::P1_PINK);
        arrow->setVisible(false);
        arrow->setLocked(true);
        arrow->setThickness(5);
        textBox = addShape<TextBox>(pos);
        textBox->setFontScale(2);
        textBox->setThickness(2);
        textBox->setOutlineColor(Colors::P1_RED);
        textBox->setLocked(true);
        registerCBs();
    }

    virtual void draw(cv::Mat &canvas);
    virtual bool mousePressed(const cv::Point &pos, bool onCreate = false);
    virtual bool isAtPos(const cv::Point &pos)
    {
        return line->isAtPos(pos);
    }
    virtual std::list<Handle *> getConnectionTargets();
    virtual const char *getType() const
    {
        return "LineCrossing";
    }

    int getDirection() const
    {
        return direction;
    }

    const std::string &getName() const
    {
        return textBox->getText();
    }

    void setName(const std::string &value) const
    {
        textBox->setText(value);
    }

    const cv::Point &getTail() const
    {
        return line->getTail();
    }

    const cv::Point &getHead() const
    {
        return line->getHead();
    }

protected:
    void registerCBs()
    {
        line->getPT1().addPosChangedCB([this](const cv::Point &)
        {
            recalcArrow();
            recalcTextBox();
        });
        line->getPT2().addPosChangedCB([this](const cv::Point &)
        {
            recalcArrow();
            recalcTextBox();
        });
    }

    void recalcArrow()
    {
        if (arrow->getVisible())
        {
            cv::Point head, tail;
            arrowTailHead(tail, head);
            arrow->setTailPos(tail);
            arrow->setHeadPos(head);
        }
    }

    void recalcTextBox()
    {
        if (textBox->getVisible())
        {
            textBox->setTL(middlePoint());
        }
    }

    cv::Point middlePoint()
    {
        const cv::Point &p1 = line->getTail();
        const cv::Point &p2 = line->getHead();
        return p1+(p2-p1)/2;
    }

    bool isMiddlePoint(const cv::Point &pos, int threshold=3)
    {
        cv::Point middle = middlePoint();
        return abs(pos.x - middle.x) <= threshold &&
                abs(pos.y - middle.y) <= threshold;
    }

    void arrowTailHead(cv::Point &tail, cv::Point &head, int mag=60)
    {
        double len = line->length()/2.;
        if (len > 0)
        {
            head = middlePoint();
            const cv::Point &p2 = line->getHead();
            // calculate the (x,y) of the left side normal unit vector
            double x = -direction*(p2.y-head.y)/len;
            double y = direction*(p2.x-head.x)/len;
            head.x=head.x+x*10;
            head.y=head.y+y*10;
            tail.x=head.x+x*mag;
            tail.y=head.y+y*mag;
        }
    }

    virtual void writeInternals(cv::FileStorage &fs) const;
    virtual void readInternals(const cv::FileNode &node);

    virtual void reloadPointers(std::list<Shape*>::const_iterator &i)
    {
        CompoundShape::reloadPointers(i);
        line = dynamic_cast<Line*>(*i++);
        arrow = dynamic_cast<Arrow*>(*i++);
        textBox = dynamic_cast<TextBox*>(*i++);
    }

private:
    Line* line;
    Arrow* arrow;
    TextBox* textBox;
    int direction;
};

}

#endif // LINECROSSING_H
