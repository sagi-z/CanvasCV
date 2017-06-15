#include "canvas.h" // 1. CanvasCV: mandatory include file

#include <iostream>

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace canvascv;

static string gHelpMsg =
"Usage:\n"
"=====\n"
"h: toggle usage message\n"
"*: toggle canvas on/off\n"
"q: exit";

// 5. CanvasCV: Using the setScreenText()
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

    // 2. CanvasCV: initializing the canvas
    Canvas c(image.size());

    // 4. CanvasCV: enabling screen text messages

    // Enabling on screen text with changes from defaults
    c.enableScreenText(Colors::Black, // fg
                       Colors::Green, // bg
                       0.6);          // font scale
    help(c);

    namedWindow("Canvas",
                WINDOW_AUTOSIZE); // diable mouse resize since resizing
                                  // the window will stretch the widgets

    int delay = 1000/15;
    int key = 0;
    Mat out; // keeping it out of the loop is a little more efficient
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

        // 3. CanvasCV: using the canvas to draw on the Mat
        c.redrawOn(image, out);

        imshow("Canvas", out);
        key = waitKeyEx(delay);
        c.consumeKey(key);
    } while (key != 'q');

    destroyAllWindows();
    return 0;
}
