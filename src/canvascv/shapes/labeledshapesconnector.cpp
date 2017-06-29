#include "labeledshapesconnector.h"

using namespace std;
using namespace cv;

namespace canvascv
{

const char * LabeledShapesConnector::type = "LabeledShapesConnector";

LabeledShapesConnector::LabeledShapesConnector(const Point &pos)
    : ShapesConnector(pos)
{
    label = addShape<TextBox>(pos);
    label->setOutlineColor(Colors::Tomato);
    center->connect(*label->getConnectionTargets().front());
}

const char *LabeledShapesConnector::getType() const {
    return type;
}

TextBox &LabeledShapesConnector::getTextBox()
{
    return *label;
}

}
