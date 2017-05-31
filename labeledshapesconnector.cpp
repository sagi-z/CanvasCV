#include "labeledshapesconnector.h"

namespace canvascv
{

const char * LabeledShapesConnector::type = "LabeledShapesConnector";

LabeledShapesConnector::LabeledShapesConnector(const Point &pos)
    : ShapesConnector(pos)
{
    label = addShape<TextBox>(pos);
    label->setLocked(true);
    label->setOutlineColor(Colors::P1_RED);
    center->connect(*label->getConnectionTargets().front());
}

}
