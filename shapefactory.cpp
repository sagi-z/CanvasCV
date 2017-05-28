#include "shapefactory.h"

namespace canvasvc
{

ShapeFactory::AllocatorsMap *ShapeFactory::allocators;

Shape *ShapeFactory::newShape(std::string type, const cv::Point &pos)
{
    AllocatorsMap::const_iterator i = allocators->find(type);
    assert (i != allocators->end());
    return i->second(pos);
}

void ShapeFactory::addShape(std::string name, ShapeFactory::Allocator a)
{
    if (! allocators)
    {
        allocators = new AllocatorsMap;
    }
    (*allocators)[name] = a;
}

}

#include "handle.h"
#include "line.h"
#include "arrow.h"
#include "textbox.h"
#include "linecrossing.h"
#include "quadrilateral.h"
#include "rectangle.h"
#include "ellipse.h"
#include "shapesconnector.h"
#include "labeledshapesconnector.h"

namespace canvasvc
{

REGISTER_SHAPE(Handle);
REGISTER_SHAPE(Line);
REGISTER_SHAPE(Arrow);
REGISTER_SHAPE(TextBox);
REGISTER_SHAPE(LineCrossing);
REGISTER_SHAPE(Quadrilateral);
REGISTER_SHAPE(Rectangle);
REGISTER_SHAPE(Ellipse);
REGISTER_SHAPE(ShapesConnector);
REGISTER_SHAPE(LabeledShapesConnector);

}
