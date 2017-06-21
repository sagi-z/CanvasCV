#include "canvas.h"

// **Optional
// These are used to create widgets
#include "widgets/radiobuttons.h"

#include <iostream>
#include <iterator>

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace canvascv;

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
    Widget::CBUserSelection cb = [](Widget *w, int index) {
        switch (index)
        {
        case 0:
            w->setOutlineColor(Colors::Blue);
            break;
        case 1:
            w->setOutlineColor(Colors::Green);
            break;
        case 2:
            w->setOutlineColor(Colors::Red);
            break;
        }
    };

    auto radioButtons = RadioButtons::create(c, {
                                                 "Blue",  // index 0
                                                 "Green", // index 1
                                                 "Red",   // index 2
                                                 "Exit"   // index 3
                                             }, 0, cb,
                                             Point(image.cols / 2., image.rows / 2.));
    radioButtons->setOutlineColor(Colors::Blue);

    namedWindow("Canvas", WINDOW_AUTOSIZE);

    // **Optional
    // This is needed for user interaction - creating/editing choosing shapes.
    // Clicking on interactive widgets.
    // We don't need this code if we only want to display "on screen" messages and "status bar" messages.
    // You can also delegate by yourself as in example_selectbox.cpp
    c.setMouseCallback("Canvas");

    int delay = 1000/15;
    int key = 0;
    Mat out; // keeping it out of the loop is a little more efficient
    while (radioButtons->getSelection() != 3 && key != 'q')
    {
        c.redrawOn(image, out);
        imshow("Canvas", out);
        key = waitKeyEx(delay); // GUI and callbacks happen here
    }

    destroyAllWindows();
    return 0;
}
