#ifndef LABELEDSHAPESCONNECTOR_H
#define LABELEDSHAPESCONNECTOR_H

#include "shapesconnector.h"
#include "textbox.h"

namespace canvasvc
{

class LabeledShapesConnector : public ShapesConnector
{
public:
    LabeledShapesConnector(const cv::Point &pos);
    virtual const char *getType() const {
        return "LabeledShapesConnector";
    }

    TextBox &getTextBox()
    {
        return *label;
    }


private:
    TextBox *label;

    virtual void reloadPointers(std::list<Shape*>::const_iterator &i)
    {
        ShapesConnector::reloadPointers(i);
        label = dynamic_cast<TextBox*>(*i++);
        center->connect(*label->getConnectionTargets().front());
    }
};

}

#endif // LABELEDSHAPESCONNECTOR_H
