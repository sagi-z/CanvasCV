Auto widget layouts {#tutlayouts}
===================
In the previous tutorials we've positioned the Widgets independetly.

In many cases you want a set of widgets to be located together in a certain order.

For this you need AutoLayout derived classes:
* canvascv::HorizontalLayout
* canvascv::VerticalLayout
* canvascv::HFrame
* canvascv::VFrame

In this tutorial we'll return to some previous tutorial code and see how
to do it in a layout. Then we'll write a simple form which gets inputs
from the user.

[TOC]

# Center text on the screen {#layouts_s1}
In this tutorial @ref screentext_s2_3 we created a Text widget and
manually tried to center it on the screen.

To center it vertically and horizontally, you would use 2 layout
managers - canvascv::VerticalLayout and canvascv::HorizontalLayout.

The top layout should stretch to the full dimensions of the screen
(because we want CENTER to be relative to that). Here we choose a
VerticalLayout, in which widgets are layered automatically veritcally,
but horizontally you can specify where to put your widget.

Now we'll add a HorizontalLayout to the above layout and request to be
anchored to the CENTER.

In the HorizontalLayout, widgets are layered automatically horizontally,
but vertically you can specify where to put your widget. This widget
will strectch in the Y direction, so CENTER in it will be relevant to
the height of the image.

So finally we'll add our Text, just like before, but now instead of
adding it to the Canvas, we'll add it to the above HorizontalLayout, and
ask to be anchored to the CENTER of it.

So the VerticalLayout will CENTER the HorizontalLayout and it will
CENTER the Text. Since the layouts are stretching to the full dimension
of the image, it will always be centered.

The added/changed code is much shorter than the explanation:
~~~~~~~{.cpp}
    using namespace canvascv;

    // ...

    auto vLayout = VerticalLayout::create(c);
    vLayout->setStretchX(true);
    vLayout->setStretchY(true);

    auto hLayout = HorizontalLayout::create(*vLayout);
    hLayout->setLayoutAnchor(Widget::CENTER);
    hLayout->setStretchY(true);

    auto txt = Text::create(*hLayout, "Target Acquired!");
    txt->setLayoutAnchor(Widget::CENTER);

    // ...
~~~~~~~

This is the full code:
~~~~~~~{.cpp}
#include <canvascv/canvas.h>
#include <canvascv/widgets/text.h>
#include <canvascv/widgets/verticallayout.h>
#include <canvascv/widgets/horizontallayout.h>

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

    Canvas c("LayoutTxt", image.size());
    c.enableScreenText(); // see it's documentation

    help(c);

    namedWindow("LayoutTxt", WINDOW_AUTOSIZE); // disable mouse resize

    auto vLayout = VerticalLayout::create(c);
    vLayout->setStretchX(true);
    vLayout->setStretchY(true);

    auto hLayout = HorizontalLayout::create(*vLayout);
    hLayout->setLayoutAnchor(Widget::CENTER);
    hLayout->setStretchY(true);

    auto txt = Text::create(*hLayout, "Target Acquired!");
    txt->setLayoutAnchor(Widget::CENTER);
    txt->setFontHeight(50);
    txt->setOutlineColor(Colors::Red);
    txt->setThickness(2);

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

        c.redrawOn(image, out);  // draw the canvas on the image copy

        imshow("LayoutTxt", out);

        key = c.waitKeyEx(); // GUI and callbacks happen here

    } while (key != 'q');

    destroyAllWindows();

    return 0;
}
~~~~~~~
* Here is a possible outcome of this code:
@image html tut_layout_txt.png
<BR>

# A simple buttons group {#layouts_s2}

Here we're going to create some buttons in a group.

The buttons are going to be evenly spaced in a VerticalLayout.

The code is simple:
~~~~~~~{.cpp}
#include <canvascv/canvas.h>
#include <canvascv/widgets/button.h>
#include <canvascv/widgets/verticallayout.h>

using namespace cv;
using namespace canvascv;

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

    namedWindow("LayoutButtons", WINDOW_AUTOSIZE); // disable mouse resize
    Canvas c("LayoutButtons", image.size());
    c.setMouseCallback();
    c.enableStatusMsg();

    auto vLayout = VerticalLayout::create(c, {100,100});

    Button::create(*vLayout,        // on the canvas
                   "First Button",  // displayed on button
                   "one");          // displayed during mouse hover

    Button::create(*vLayout,        // on the canvas
                   "Second Button", // displayed on button
                   "two");          // displayed during mouse hover

    Button::create(*vLayout,        // on the canvas
                   "Third Button",  // displayed on button
                   "three");        // displayed during mouse hover

    int key = 0;
    c.setImage(image);
    do
    {
        key = c.waitKeyEx(); // GUI and callbacks happen here

    } while (key != 'q');

    destroyAllWindows();

    return 0;
}
~~~~~~~
Notes:
* When creating widgets the first parameter is the containing layout
(the Canvas is also a Layout).
* As you can see, there is a possible problem with this code - the
length of the buttons:
@image html tut_layout_buttons1.png
* To fix this, add these lines after the creation of the buttons:
~~~~~~~{.cpp}
    using namespace canvascv;
    // ...

    vLayout->at<Button>(0)->setStretchX(true);
    vLayout->at<Button>(1)->setStretchX(true);
    vLayout->at<Button>(2)->setStretchX(true);

    // ...
~~~~~~~
* Notice how we can access the items ina layout by their index.
* Now the buttons have the same length:
@image html tut_layout_buttons2.png
<BR>

# A full dialog {#layouts_s3}
Now it's time to take all the widgets we've been talking about in the
tutorials and make a dialog frame for out user.

As always it's going to be displayed on an image, but this time we'll
make sure the image size fits into the screen. Since OpenCV doesn't
expose the desktop size, we'll hard code a rough estimation of 1024x768
maximum size.

* The dialog will be in a HFrame.
  * top - title
  * bottom - body, split into 2 vertical parts.
    * Left part with will have
      * RadioButtons
      * CheckBoxes
    * Right part with will have
      * Sunken Frame with Text to display RadioButtons and CheckBoxes selection (TOP)
      * Ok/Cancel (BOTTOM anchored)

