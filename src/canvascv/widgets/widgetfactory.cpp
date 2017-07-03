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
    shared_ptr<Widget> widget(i->second(pos));
    ThemeRepository::applyCurrentTheme(widget.get());
    layoutVal.addWidget(widget);
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

CCV_REGISTER_WIDGET(Text);
CCV_REGISTER_WIDGET(Button);
CCV_REGISTER_WIDGET(VerticalLayout);
CCV_REGISTER_WIDGET(HorizontalLayout);
CCV_REGISTER_WIDGET(VFrame);
CCV_REGISTER_WIDGET(HFrame);
CCV_REGISTER_WIDGET(MsgBox);
CCV_REGISTER_WIDGET(SelectionBox);
CCV_REGISTER_WIDGET(MatWidget);
CCV_REGISTER_WIDGET(RadioButtons);
CCV_REGISTER_WIDGET(CheckBoxes);

}
