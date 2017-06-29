Leaving the console {#tutscreentext}
===================
OpenCV examples and tutorials make heavy use of the console to commuincate with the user.

To start using the GUI instead of the console the first thing you need is to get to know the Canvas class.

Introducing the CanvasCV class
------------------------------
The Canvas class is associated with an OpenCV window and it gives you another virtual layer on top of your displayed Mat.

This canvas class encapsulates **a lot** of work for you, so you can focus on the CV. It also handles key presses
and mouse events.

The basic flow is:
* Define a Canvas of a specific size, usually to match the image you're displayng.
~~~~~~~{.cpp}
Canvas c(frame.size());
~~~~~~~

* Create an OpenCV window without resizing
~~~~~~~{.cpp}
    namedWindow("MyWindowName", WINDOW_AUTOSIZE); // disable mouse resize
~~~~~~~

* Let the CanvasCV handle your mouse events
~~~~~~~{.cpp}
    c.setMouseCallback("MyWindowName"); // optional for mouse usage (see also example_selectbox.cpp)
~~~~~~~

* Create you widget on the Canvas (or in a frame/layout) for example:
~~~~~~~{.cpp}
    auto msgBox = MsgBox::create(c, "Do you really want to do that?", {"Yes", "No"});
~~~~~~~

* Redraw on your frame/image in your regular OpenCV loop, as in:
~~~~~~~{.cpp}
    int delay = 1000/25;
    int key = -1;
    Mat out;
    while (true)
    {
        c.redrawOn(frame, out);   // draws your virtual GUI layer on top of the 'frame'
        imshow("MyWindowName", out);
        key = c.waitKeyEx(delay); // forwards key presses to widgets and shapes
    }
~~~~~~~

Some real use cases
-------------------
###Exiting on fatal errors
Your application/utility might need command line arguments, or cannot continue for some reason.
 A simple write to the console before exiting is usually not enough to get the user attention.  
For these simple cases you have a simple shortcut in Canvas::fatal():
~~~~~~~{.cpp}
#include "canvascv/canvas.h"

using namespace canvascv;

int main(int argc, char **argv)
{
    --argc;
    ++argv;
    if (! argc)
    {
        Canvas::fatal("Must get a path to an image as a parameter" , -1);
    }
    return 0;
}
~~~~~~~

which gives you:
@image html tut_fatal.png

