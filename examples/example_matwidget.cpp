#include "canvascv/canvas.h"

// These are used to create widgets
#include "canvascv/widgets/matwidget.h"

#include <iostream>
#include <iterator>

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace canvascv;

shared_ptr<MatWidget> matWidget;

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
        if (matWidget->getVisible() == false)
        {
            matWidget->setLocation(pos);
            matWidget->setVisible(true);
        }
        else if (! pCanvas->onMousePress(pos))
        {
            matWidget->setLocation(pos);
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

static Mat drawSmily()
{
    Mat smily(48,48,CV_8UC4);
    smily = Scalar::all(0); // transparent BG
    Scalar orange = Colors::Orange;
    orange[3] = 255; // opaque FG
    circle(smily, {23,23}, 23, orange, 2, LINE_AA);
    circle(smily, {15,15}, 3, orange, 2, LINE_AA);
    circle(smily, {33,15}, 3, orange, 2, LINE_AA);
    ellipse(smily, {23,33}, Size(6,3), 0., 10., 170., orange, 2, LINE_AA);
    return smily;
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
    Mat smily = drawSmily();
    matWidget = MatWidget::create(c, smily);
    matWidget->setVisible(false);
    matWidget->setAlpha(0.);

    namedWindow("Canvas", WINDOW_AUTOSIZE);
    setMouseCallback("Canvas", mouseCB, &c);

    int key = 0;
    Mat out; // keeping it out of the loop is a little more efficient
    while (key != 'q')
    {
        c.redrawOn(image, out);
        imshow("Canvas", out);
        key = c.waitKeyEx(); // GUI and callbacks happen here
    }

    destroyAllWindows();
    return 0;
}
