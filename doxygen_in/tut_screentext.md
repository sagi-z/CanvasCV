Leaving the console {#tutscreentext}
===================
OpenCV examples and tutorials make heavy use of the console to commuincate with the user.

To start using the GUI instead of the console the first thing you need is to get to know the canvascv::Canvas class.

[TOC]

@section screentext_s1 Introducing the CanvasCV class
The Canvas class is associated with an OpenCV window and it gives you another virtual layer on top of your displayed Mat.

This canvas class encapsulates **a lot** of work for you, so you can focus on the CV. It also handles key presses
and mouse events.

The basic flow is:
* Define a Canvas for a window of a specific size, usually to match the image you're displayng.
~~~~~~~{.cpp}
Canvas c("winName", frame.size());
~~~~~~~

* Create an OpenCV window without resizing so the widgets cannot be stretched
~~~~~~~{.cpp}
    namedWindow("winName", WINDOW_AUTOSIZE); // disable mouse resize
~~~~~~~

* Let the CanvasCV handle your mouse events
~~~~~~~{.cpp}
    c.setMouseCallback(); // optional for mouse usage (see also example_selectbox.cpp)
~~~~~~~

* Create you widget on the Canvas (or in a frame/layout) for example:
~~~~~~~{.cpp}
    auto msgBox = MsgBox::create(c, "Do you really want to do that?", {"Yes", "No"});
~~~~~~~

* Redraw on your frame/image in your regular OpenCV loop, as in:
~~~~~~~{.cpp}
    int delay = 1000/25; // for a single image, delay should be 0
    int key = -1;
    Mat out;
    while (true)
    {
        c.redrawOn(frame, out);   // draws your virtual GUI layer on top of the 'frame'
        c.imshow(out);            // you can also use imshow directly for "winName"
        key = c.waitKeyEx(delay); // forwards key presses to widgets and shapes
    }
~~~~~~~

@section screentext_s2 Some real use cases
@subsection screentext_s2_1 Exiting on fatal errors
Your application/utility might need command line arguments, or cannot continue for some reason.

A simple write to the console before exiting is usually not enough to get the user attention.  

For these simple cases you have a simple shortcut in Canvas::fatal():
~~~~~~~{.cpp}
#include <canvascv/canvas.h>

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

which, besides an output to STDERR, gives you:
@image html tut_fatal.png
<BR>

@subsection screentext_s2_2 Displaying user text in a fixed location
Instead of displaying the help message or any other fixed text to the console use the enableScreenText and setScreenText(string) methods.

This flow is a little closer to the regular usage of the Canvas, but you don't need to create a widget of your own.

Adding to the previous example (we want to display this on an image):
~~~~~~~{.cpp}
#include <canvascv/canvas.h>

using namespace canvascv;

void help(Canvas &c)
{
    static bool showHelp = true;
    static string helpMsg =
            "Usage:\n"
            "=====\n"
            "h: toggle usage message\n"
            "*: toggle canvas on/off\n"
            "q: exit";


    if (showHelp) c.setScreenText(helpMsg);
    else c.setScreenText("");

    showHelp = ! showHelp;
}

int main(int argc, char **argv)
{
    --argc;
    ++argv;
    if (! argc)
    {
        Canvas::fatal("Must get a path to an image as a parameter" , -1);
    }

    Mat image = imread(argv[0]);
    if (image.empty())
    {
        Canvas::fatal(string("Cannot load image ") + argv[0], -2);
    }

    Canvas c("Canvas", image.size());
    c.enableScreenText(); // see it's documentation

    help(c);

    namedWindow("Canvas", WINDOW_AUTOSIZE); // disable mouse resize

    int key = 0;
    Mat out;
    do
    {
        switch (key)
        {
        case 'h':
            help(c);
            break;
        case '*':
            c.setOn(! c.getOn());
            break;
        }

        c.redrawOn(image, out);   // draw the canvas on the image copy

        imshow("Canvas", out);    // using cv::imshow works fine

        key = c.waitKeyEx(); // GUI and callbacks happen here

    } while (key != 'q');

    destroyAllWindows();

    return 0;
}
~~~~~~~

Some notes here:
* Note the continued use of canvascv::Canvas::fatal().
* Note that we didn't need to use canvascv::Canvas::setMouseCallback() or create a widget of our own here.
* When you enableScreenText() you can configure it's display.
* As you can see the Canvas instance can be completely turned on and off with the canvascv::Canvas::setOn() method.
* canvascv::Canvas::waitKeyEx knows to update it's internal shapes and
  widgets even if you pass 0 as a blocking delay indicator.
* When executed with a path to an image, this gives you (depends on your image):
@image html tut_screentext.png
<BR>

@subsection screentext_s2_3 Displaying text where ever you want
To display text where you want you need the canvascv::Text widget.

You can create position it on the Canvas at any XY location.

Add these lines to the previous example, after creating the namedWindow():
~~~~~~~{.cpp}
using namespace canvascv;
    //...

    auto txt = Text::create(c, "Target Acquired!");
    txt->setFontHeight(50);
    txt->setOutlineColor(Colors::Red);
    txt->setThickness(2);
    txt->setLocation({30, image.rows / 2 - 15});

    //...
~~~~~~~

Notes:
* All widgets have a static create methods, which is the only way to create them.
* The canvascv::Text::create will return a shared_ptr<Text> instance, which you don't have to keep since another one is kept by the canvascv::Layout.
* There are many Colors constants to choose from, see canvascv::Colors.
* The canvascv::Widget::setLocation gives specific XY postion, but a canvascv::HorizontalLayout could help us put this in the CENTER.
* When executed with a path to an image, this gives you (depends on your image):
@image html tut_text.png
<BR>

**That's all for this tutorial**
