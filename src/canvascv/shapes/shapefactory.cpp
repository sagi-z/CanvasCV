#include "shapefactory.h"

namespace canvascv
{

ShapeFactory::AllocatorsMap *ShapeFactory::allocators;

Shape *ShapeFactory::newShape(std::string type, const cv::Point &pos)
{
    AllocatorsMap::const_iterator i = allocators->find(type);
    assert (i != allocators->end());
    Shape *shape = i->second(pos);
    ThemeRepository::applyCurrentTheme(shape);
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

CCV_REGISTER_SHAPE(Handle);
CCV_REGISTER_SHAPE(Line);
CCV_REGISTER_SHAPE(Arrow);
CCV_REGISTER_SHAPE(TextBox);
CCV_REGISTER_SHAPE(LineCrossing);
CCV_REGISTER_SHAPE(Polygon);
CCV_REGISTER_SHAPE(Rectangle);
CCV_REGISTER_SHAPE(Ellipse);
CCV_REGISTER_SHAPE(ShapesConnector);
CCV_REGISTER_SHAPE(LabeledShapesConnector);

}
