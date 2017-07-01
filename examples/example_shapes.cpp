// Mandator include file
#include "canvascv/canvas.h"

#include <iostream>
#include <iterator>

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace canvascv;

static string gHelpMsg =
"Usage:\n"
"=====\n"
"Use these keys:\n"
"1: Line\n"
"2: Arrow\n"
"3: TextBox\n"
"4: LineCrossing\n"
"5: Polygon\n"
"6: Rectangle\n"
"7: Ellipse\n"
"8: ShapesConnector\n"
"9: LabeledShapesConnector\n"
"h: toggle usage message\n"
"*: toggle canvas on/off\n"
"c: clear the shapes from the canvas\n"
"s: save shapes to config.xml\n"
"l: load shapes from config.xml\n"
"DEL: delete active shape\n"
"q: exit";

void help(Canvas &c)
{
    static bool showHelp = true;
    if (showHelp)
    {
        cout << gHelpMsg << endl;
        c.setScreenText(gHelpMsg);
    }
    else
    {
        c.setScreenText("");
    }
    showHelp = ! showHelp;
}

int main(int argc, char **argv)
{
    --argc;
    ++argv;
    Mat image;
    if (argc)
    {
        Mat orig = imread(argv[0], IMREAD_UNCHANGED);
        if (orig.empty())
        {
            Canvas::fatal(string("Cannot load image ") + argv[0], -1);
        }
        if (orig.cols > 1024)
        {
            double ratio = 1024. / orig.cols;
            cv::resize(orig, image, Size(), ratio, ratio);
        }
        else
        {
            image = orig;
        }
    }
    else
    {
        image.create(600, 800, CV_8UC3);
        image = Colors::White;
    }

    Canvas c("Canvas", image.size());
    c.setShapeType("Line"); // default shape type for direct GUI creation
    c.enableScreenText();
    c.enableStatusMsg();
    c.setDefaultStatusMsg("Click-Release-Drag to create a new shape.\nSelect/Unselect with the mouse.\nDrag selected shapes with the mouse.");
    help(c);

    namedWindow("Canvas", WINDOW_AUTOSIZE);

    c.setMouseCallback(); // optional for mouse usage see also (example_selectbox.cpp)

    int delay = 1000/25;
    int key = 0;
    Mat out; // keeping it out of the loop is a little more efficient
    do
    {
        switch (key)
        {
        case '1':
            c.setShapeType("Line");
            break;
        case '2':
            c.setShapeType("Arrow");
            break;
        case '3':
            c.setShapeType("TextBox");
            break;
        case '4':
            c.setShapeType("LineCrossing");
            break;
        case '5':
            c.setShapeType("Polygon");
            break;
        case '6':
            c.setShapeType("Rectangle");
            break;
        case '7':
            c.setShapeType("Ellipse");
            break;
        case '8':
            c.setShapeType("ShapesConnector");
            break;
        case '9':
            c.setShapeType("LabeledShapesConnector");
            break;
        case 'h':
            help(c);
            break;
        case '*':
            c.setOn(! c.getOn());
            break;
        case 'c':
            c.clearShapes();
            break;
        case 's':
        {
            c.writeShapesToFile("config.xml");
        }
            break;
        case 'l':
        {
            c.readShapesFromFile("config.xml");
        }
            break;
        case 65535:
            c.deleteActive();
            break;
        }

        c.redrawOn(image, out);
        imshow("Canvas", out);
        key = c.waitKeyEx(delay); // GUI and callbacks happen here
    } while (key != 'q');

    destroyAllWindows();
    return 0;
}
