#include "canvas.h"

// **Optional
// These are used to create widgets
#include "widgets/selectionbox.h"

#include <iostream>
#include <iterator>

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace canvascv;

shared_ptr<SelectionBox> selectionBox;
// **Optional
// This is needed for user interaction - creating/editing choosing shapes.
// Clicking on interactive widgets.
// We don't need this code if we only want to display "on screen" messages and "status bar" messages.
static void mouseCB(int event, int x, int y, int flags, void* userData) {
    (void)flags;
    Canvas *pCanvas=reinterpret_cast<Canvas*>(userData);
    Point pos(x,y);
    switch( event )
    {
    case EVENT_LBUTTONDOWN:
        if (selectionBox->getVisible() == false)
        {
            selectionBox->setLocation(pos);
            selectionBox->setVisible(true);
        }
        else if (! pCanvas->onMousePress(pos))
        {
            selectionBox->setLocation(pos);
        }
        break;
    case EVENT_LBUTTONUP:
        pCanvas->onMouseRelease(pos);
        break;
    case EVENT_MOUSEMOVE:
        pCanvas->onMouseMove(pos);
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
    selectionBox = SelectionBox::create(c, {
                                            "Long Option1",     // index 0
                                            "Option2\n2 lines", // index 1
                                            "Option3"           // index 2
                                        },
                                        [](Widget *w, int i) {
        w->setVisible(false);
        cout << "Option " << i << " was chosen" << endl;
    });
    selectionBox->setVisible(false);

    namedWindow("Canvas", WINDOW_AUTOSIZE);
    setMouseCallback("Canvas", mouseCB, &c);

    c.enableScreenText();
    c.setScreenText("left click to open selection box. left click to select an item");

    int delay = 1000/25;
    int key = 0;
    Mat out; // keeping it out of the loop is a little more efficient
    while (key != 'q')
    {
        c.redrawOn(image, out);
        imshow("Canvas", out);
        key = waitKeyEx(delay); // GUI and callbacks happen here
    }

    destroyAllWindows();
    return 0;
}
