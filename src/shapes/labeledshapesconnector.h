#ifndef LABELEDSHAPESCONNECTOR_H
#define LABELEDSHAPESCONNECTOR_H

#include "shapesconnector.h"
#include "textbox.h"

namespace canvascv
{

class LabeledShapesConnector : public ShapesConnector
{
public:

    virtual const char *getType() const;

    TextBox &getTextBox();

    static const char * type;

private:
    friend class ShapeFactory;
    template <class T> friend class ShapeFactoryT;

    LabeledShapesConnector(const cv::Point &pos);

    TextBox *label;

    virtual void reloadPointers(const std::list<Shape*> &lst, std::list<Shape*>::const_iterator &i)
    {
        ShapesConnector::reloadPointers(lst, i);
        label = dynamic_cast<TextBox*>(*i++);
        center->connect(*label->getConnectionTargets().front());
    }
};

}

#endif // LABELEDSHAPESCONNECTOR_H
