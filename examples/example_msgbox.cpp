#include "canvascv/canvas.h"

// These are used to create widgets
#include "canvascv/widgets/msgbox.h"

#include <iostream>
#include <iterator>

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace canvascv;

// This is needed for user interaction - creating/editing choosing shapes.
// Clicking on interactive widgets.
// We don't need this code if we only want to display "on screen" messages and "status bar" messages.
static void mouseCB(int event, int x, int y, int flags, void* userData) {
    (void)flags;
    Canvas *pCanvas=reinterpret_cast<Canvas*>(userData);
    switch( event )
    {
    case EVENT_LBUTTONDOWN:
        pCanvas->onMousePress(Point(x,y));
        break;
    case EVENT_LBUTTONUP:
        pCanvas->onMouseRelease(Point(x,y));
        break;
    case EVENT_MOUSEMOVE:
        pCanvas->onMouseMove(Point(x,y));
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
        if (orig.empty()) {
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
        Canvas::fatal("Must get a path to an image as a parameter" , -1);
    }

    Canvas c("Canvas", image.size());
    auto msgBox = MsgBox::create(c,
                                 "This is a MsgBox example\n"
                                 "with 2 lines", {
                                     "Ok",           // index 0
                                     "Cancel",       // index 1
                                     "Somthing else" // index 2
                                 });

    namedWindow("Canvas", WINDOW_AUTOSIZE);
    setMouseCallback("Canvas", mouseCB, &c);

    int key = 0;
    int delay = 1000/25;  // if using a polling API we need a delay
    Mat out; // keeping it out of the loop is a little more efficient
    while (msgBox && key != 'q')
    {
        if (msgBox->getUserSelection() != -1)
        {
            int selection = msgBox->getUserSelection();
            cout << "MsgBox was pressed with key index " << selection <<
                    "(" << msgBox->getTextAt(selection) << ")" << endl;
            msgBox = MsgBox::create(c,
                                    "Do you really want to exit?",
                                    {"Yes", "No"},
                                    [&c, &key, &msgBox](Widget *, int i) {
                if (i == 0) msgBox.reset();
                else msgBox = MsgBox::create(c, "Just another MsgBox\nWaiting for exit approval");
            });
        }
        c.redrawOn(image, out);
        imshow("Canvas", out);
        key = c.waitKeyEx(delay); // GUI and callbacks happen here
    }

    destroyAllWindows();
    return 0;
}
