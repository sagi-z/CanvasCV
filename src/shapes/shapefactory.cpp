#include "shapefactory.h"

namespace canvascv
{

ShapeFactory::AllocatorsMap *ShapeFactory::allocators;

Shape *ShapeFactory::newShape(std::string type, const cv::Point &pos)
{
    AllocatorsMap::const_iterator i = allocators->find(type);
    assert (i != allocators->end());
    Shape *shape = i->second(pos);
    ThemeRepository::instance()->applyCurrentTheme(shape);
    return shape;
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
#include "polygon.h"
#include "rectangle.h"
#include "ellipse.h"
#include "shapesconnector.h"
#include "labeledshapesconnector.h"

namespace canvascv
{

REGISTER_SHAPE(Handle);
REGISTER_SHAPE(Line);
REGISTER_SHAPE(Arrow);
REGISTER_SHAPE(TextBox);
REGISTER_SHAPE(LineCrossing);
REGISTER_SHAPE(Polygon);
REGISTER_SHAPE(Rectangle);
REGISTER_SHAPE(Ellipse);
REGISTER_SHAPE(ShapesConnector);
REGISTER_SHAPE(LabeledShapesConnector);

}
