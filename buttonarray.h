#ifndef BUTTONARRAY_H
#define BUTTONARRAY_H

#include "compoundwidget.h"

namespace canvascv
{

class ButtonArray : public CompoundWidget
{
public:
    enum Orientation
    {
        VERTICAL,
        HORIZONTAL
    };

    ButtonArray();

private:
    int spacing;
};

}

#endif // BUTTONARRAY_H
