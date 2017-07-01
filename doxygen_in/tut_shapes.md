Creating shapes {#tutshapes}
===============

CanvasCV's support of shapes is one of it's strongest points.

You have 2 ways of creating shapes:
* From the GUI, with the mouse and keyboard - for example to let the
user select a polygon mask as an area of intereset for the CV.
* From the code - for example creating a clickable rectangle on screen
to which a SelectionBox widget can be applied.

[TOC]

# Creating a polygon from the GUI {#tshsec1}
In this example we want to user to mark a polygon area on the image or
frame.

There could be various reasons for that:
* Since the camera is capturing a large area and we want to operate
our algorithm on a relevant dynamic subset of it.
* The user selects a polygon area that triggers an alert when tracked
objects are inside it.
* etc.

When the user creates/modifies/deletes the polygon we'll just print the
vertices to the screen.

Here is the code:
~~~~~~~{.cpp}
#include "canvascv/canvas.h"
#include "canvascv/shapes/polygon.h"

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace canvascv;

void handlePolyPoints(Canvas &c, Polygon &poly)
{
    vector<Point> polyPoints;
    poly.getPoints(polyPoints);
    stringstream s;
    s <<  "polygon points are at :\n[";
    for (auto &v : polyPoints)
    {
        s << " " << v << " ";
    }
    s <<  "]\n\n";
    s <<  "Use q to exit.";
    c.setScreenText(s.str());
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

    Canvas c("Shapes", image.size());
    c.enableScreenText();

    c.enableStatusMsg();
    c.setDefaultStatusMsg("Click-Release-Drag to create a single polygon.\n"
                          "Select/Unselect with the mouse.\n"
                          "Drag selected polygon with the mouse.\n"
                          "DEL deleted the selected shape.");

    c.setShapeType(Polygon::type); // default shape type for direct GUI creation
    c.notifyOnShapeCreate([&c](Shape* shape)
    {
        handlePolyPoints(c, *(Polygon*) shape); // it can only be a Polygon
        c.setShapeType(""); // default shape type for direct GUI creation
    });
    c.notifyOnShapeModify([&c](Shape* shape)
    {
        handlePolyPoints(c, *(Polygon*) shape);
    });
    c.notifyOnShapeDelete([&c](Shape*)
    {
        c.setShapeType(Polygon::type); // default shape type for direct GUI creation
    });

    namedWindow("Shapes", WINDOW_AUTOSIZE | WINDOW_GUI_NORMAL );
    c.setMouseCallback(); // optional for mouse usage see also (example_selectbox.cpp)

    int delay = 1000/25;
    int key = 0;
    Mat out; // keeping it out of the loop is a little more efficient
    do
    {
        switch (key)
        {
        case 65535:
            c.deleteActive();
            c.setScreenText("");
            break;
        }

        c.redrawOn(image, out);
        c.imshow(out);
        key = c.waitKeyEx(delay); // GUI and callbacks happen here
    } while (key != 'q');

    destroyAllWindows();
    return 0;
}
~~~~~~~
Notes:
* canvascv::Canvas::enableStatusMsg() is optional, but recommended when
using shapes and widgets. It takes a small portion of the screen area,
but gives helpful information to the user.
* canvascv::Canvas::setDefaultStatusMsg() is optional. There are built
in status messages that are displayed during shape creation and editing.
When nothing is selected, then your default status message will be
displayed.
* The Canvas needs to know what shape to create when the user left
clicks with the mouse. This is done with
canvascv::Canvas::setShapeType().
* You can register for Canvas create/modify/delete notification on
the Shapes that it creates/modify/deletes. This is the place to react to
the user's GUI actions.
* When executed with a path to an image, this gives you (depends on your image):
@image html tut_shapes_gui.png
<BR>

# Creating a clickable rectangle from code {#tshsec2}

**That's all for this tutorial**
