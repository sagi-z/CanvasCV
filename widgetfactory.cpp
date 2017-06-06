#include "widgetfactory.h"

namespace canvascv
{

WidgetFactory::AllocatorsMap *WidgetFactory::allocators;

Widget *WidgetFactory::newWidget(std::string type, const cv::Point &pos)
{
    AllocatorsMap::const_iterator i = allocators->find(type);
    assert (i != allocators->end());
    return i->second(pos);
}

void WidgetFactory::addWidget(std::string name, WidgetFactory::Allocator a)
{
    if (! allocators)
    {
        allocators = new AllocatorsMap;
    }
    (*allocators)[name] = a;
}

}

#include "floatingtext.h"
#include "button.h"
#include "verticallayout.h"

namespace canvascv
{

REGISTER_WIDGET(FloatingText);
REGISTER_WIDGET(Button);
REGISTER_WIDGET(VerticalLayout);

}
