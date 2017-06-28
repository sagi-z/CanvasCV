// Mandator include file
#include "canvas.h"

// **Optional
// We need specific linecrossing and arrow features here
#include "shapes/linecrossing.h"
#include "shapes/arrow.h"

#include <iostream>
#include <iterator>

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace canvascv;

static string gHelpMsg =
"Usage:\n"
"=====\n"
"Use the mouse and keyboard.\n"
"Click and drag to create and drag arrows\n"
"acrooss the LineCrossing.\n"
"Change the LineCrossing direction and\n"
"create/drag the arrows again.\n"
"Use these keys:\n"
"h: toggle usage message\n"
"DEL: delete active shape (deleting the\n"
"LineCrossing will exit)\n"
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

void setArrowColors(LineCrossing &lc, Arrow &arrow, Scalar defaultColor)
{
    Handle &tail = arrow.getPT1();
    Handle &head = arrow.getPT2();
    int crossing = lc.isCrossedBySegment(tail(), head());
    switch (crossing)
    {
    case 1:
        arrow.setOutlineColor(Colors::Green);
        break;
    case -1:
        arrow.setOutlineColor(Colors::Red);
        break;
    default:
        arrow.setOutlineColor(defaultColor);
        break;
    }
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
            cerr << "Cannot load image " << argv[0] << endl;
            return -1;
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

    Canvas c(image.size());
    c.setShapeType("Arrow"); // default shape type for direct GUI creation
    c.enableScreenText();
    c.enableStatusMsg();
    help(c);

    // create the line-crossing
    shared_ptr<LineCrossing> lc = c.createShape<LineCrossing>();
    lc->getLine()->setTailPos(Point(image.cols / 2, image.rows * 0.75));
    lc->getLine()->setHeadPos(Point(image.cols / 2, image.cols * 0.25));

    // notifyOnCreate callbacks can be used for many things.
    // Here we bind Arrow modificatons and LineCrossing modifications
    // to call setArrowColors()
    c.notifyOnShapeCreate([lc, &c](Shape *arrowShape)
    {
        if (arrowShape->getType() == Arrow::type)
        {
            Arrow *arrow = (Arrow*) arrowShape;
            Scalar defaultColor = arrowShape->getOutlineColor();
            Handle::PosChangedCB cb = [arrow, defaultColor, lc](const Point&)
            {
                setArrowColors(*lc, *arrow, defaultColor);
            };
            arrow->getPT1().addPosChangedCB(cb);
            arrow->getPT2().addPosChangedCB(cb);

             // register for LineCrossing's directional arrow changes
            auto cbId = lc->getArrow()->getPT1().addPosChangedCB(cb);

            // If an arrow is deleted, don't let the LineCrossing's directional
            //  arrow notify it about changes
            int shapeId = lc->getArrow()->getPT1().getId();
            c.notifyOnShapeDelete([&c, arrow, shapeId, cbId](Shape *deleted)
            {
                if (arrow == deleted)
                {
                    auto shapePtr = c.getShape(shapeId);
                    if (shapePtr)
                    {
                        ((Handle*)shapePtr.get())->delPosChangedCB(cbId);
                    }
                }
            });
        }
    });

    // when the LiceCrossing is reset the while loop will exit
    c.notifyOnShapeDelete([&lc](Shape *shape) { if (shape->getType() == LineCrossing::type) lc.reset(); });

    namedWindow("Canvas", WINDOW_AUTOSIZE);
    c.setMouseCallback("Canvas"); // optional for mouse usage see also (example_selectbox.cpp)

    int delay = 1000/25;
    int key = 0;
    Mat out; // keeping it out of the loop is a little more efficient
    while (lc && key != 'q')
    {
        switch (key)
        {
        case 'h':
            help(c);
            break;
        case 65535:
            c.deleteActive();
            break;
        }

        c.redrawOn(image, out);
        imshow("Canvas", out);
        key = c.waitKeyEx(delay); // GUI and callbacks happen here
    }

    destroyAllWindows();
    return 0;
}
