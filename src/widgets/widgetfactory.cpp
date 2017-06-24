#include "widgetfactory.h"

using namespace std;
using namespace cv;

namespace canvascv
{

WidgetFactory::AllocatorsMap *WidgetFactory::allocators;

shared_ptr<Widget> WidgetFactory::newWidget(string type, Layout &layoutVal, const Point &pos)
{
    AllocatorsMap::const_iterator i = allocators->find(type);
    assert (i != allocators->end());
    shared_ptr<Widget> widget(i->second(layoutVal, pos));
    postConstuct(layoutVal, widget);
    ThemeRepository::instance()->applyCurrentTheme(widget.get());
    return widget;
}

void WidgetFactory::addWidget(string name, WidgetFactory::Allocator a)
{
    if (! allocators)
    {
        allocators = new AllocatorsMap;
    }
    (*allocators)[name] = a;
}

bool WidgetFactory::postConstuct(Layout &layout, const std::shared_ptr<Widget> &widget)
{
     bool mustBeTrue = layout.replaceTmpSharedPtr(widget);
     assert(mustBeTrue);
     return mustBeTrue;
}

}

#include "text.h"
#include "button.h"
#include "verticallayout.h"
#include "horizontallayout.h"
#include "vframe.h"
#include "hframe.h"
#include "msgbox.h"
#include "selectionbox.h"
#include "matwidget.h"
#include "radiobuttons.h"
#include "checkboxes.h"

namespace canvascv
{

REGISTER_WIDGET(Text);
REGISTER_WIDGET(Button);
REGISTER_WIDGET(VerticalLayout);
REGISTER_WIDGET(HorizontalLayout);
REGISTER_WIDGET(VFrame);
REGISTER_WIDGET(HFrame);
REGISTER_WIDGET(MsgBox);
REGISTER_WIDGET(SelectionBox);
REGISTER_WIDGET(MatWidget);
REGISTER_WIDGET(RadioButtons);
REGISTER_WIDGET(CheckBoxes);

}
