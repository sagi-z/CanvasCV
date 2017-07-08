#ifndef LINECROSSING_H
#define LINECROSSING_H

#include "arrow.h"
#include "line.h"
#include "textbox.h"
#include "shapefactory.h"
#include <memory>

namespace canvascv
{

/**
 * @brief The LineCrossing class
 * 
 * - Helps us know when something we tracked passed over a line.
 * - We also need to know if it passed in one direction or another.
 */
class LineCrossing : public CompoundShape
{
public:

    /// -1 or 1
    int getDirection() const;

    /// gets the text from the attached TextBox
    const std::string &getName() const;

    /// sets the text in the attached TextBox
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

    /**
     * @brief wasCrossed
     * 
     * Use cross product and predefined direction to tell if a given point
     * is accross the <b> endless line </b> represented by this segment.
     * @param pt is the point we're going to examine
     * @return bool if line is on the other side of the line according to the direction arrow
     */
    bool wasCrossed(const Point &pt) const;

    /**
     * @brief isCrossedBy
     * 
     * Use cross product and predefined direction to tell if a given line
     * path, starting at lineStart and ending at lineEnd is crossing this
     * specific <b> line segment </b>.
     * @param lineStart the start of the line to check against (path origin Point).
     * @param lineEnd the end of the line to check against (path latest Point).
     * @return
     * -  0 if the segments are not crossing
     * -  1 if the segment crossed according to direction
     * - -1 if the segment crossed against the direction
     */
    int isCrossedBySegment(const Point &lineStart, const Point &lineEnd) const;

    virtual bool isAtPos(const cv::Point &pos)
    {
        return line->isAtPos(pos);
    }

    virtual std::list<Handle *> getConnectionTargets();
    virtual const char *getType() const;
    static const char * type;

protected:
    friend class ShapeFactory;
    template <class T> friend class ShapeFactoryT;

    LineCrossing(const cv::Point& pos);

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

    virtual const std::string &getEditStatusMsg() const;

    int cross_z(const Point &pt) const;
    int cross_z(const Point &lineStart, const Point &lineEnd, int direction, const Point &pt) const;

private:
    Line* line;
    Arrow* arrow;
    TextBox* textBox;
    int direction;
    int arrowMagnitude;
};

}

/** @example example_linecrossing.cpp
 * This is an example of how to use the LineCrossing shape.
 */

#endif // LINECROSSING_H
