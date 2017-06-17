#include "canvas.h"

// **Optional
// These are used to create widgets
#include "widgets/msgbox.h"

#include <iostream>
#include <iterator>

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace canvascv;

// **Optional
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
        Mat orig = imread(argv[0]);
        if (orig.empty()) {
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
        cout << "Must get a path to an image as a parameter" << endl;
        return -1;
    }

    Canvas c(image.size());
    auto msgBox = MsgBox::create(c,
                                 "This is a MsgBox example\n"
                                 "with 2 lines", {
                                     "Ok",           // 0
                                     "Cancel",       // 1
                                     "Somthing else" // 2
                                 });

    namedWindow("Canvas", WINDOW_AUTOSIZE);
    setMouseCallback("Canvas", mouseCB, &c);

    int delay = 1000/15;
    int key = 0;
    Mat out; // keeping it out of the loop is a little more efficient
    do
    {
        if (msgBox->getUserSelection() != -1)
        {
            cout << "MsgBox was pressed with key index " << msgBox->getUserSelection() << endl;
        }
        c.redrawOn(image, out);
        imshow("Canvas", out);
        key = waitKeyEx(delay);
    } while (key != 'q');

    destroyAllWindows();
    return 0;
}
