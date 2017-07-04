#include <canvascv/canvas.h>
#include <canvascv/widgets/button.h>

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

    Mat orig = imread(argv[0]);
    if (orig.empty())
    {
        Canvas::fatal(string("Cannot load image ") + argv[0], -2);
    }
    Mat image = orig;

    namedWindow("Buttons", WINDOW_AUTOSIZE); // disable mouse resize
    Canvas c("Buttons", orig.size());
    c.enableStatusMsg(); // shows hover over button msgs
    c.setMouseCallback();

    double ratio = 1.;

    auto rightSideButton = Button::create(c,                                 // on the canvas
                                          Point(image.cols - 20, 0),         // top-right aligned on the screen
                                          "+",                               // displayed on button
                                          "increase size",                   // displayed during mouse hover
                                          [&c, &orig, &image, &ratio](Widget *w) // callback can be a C++11 lambda
    {
        ratio += 0.05;
        cv::resize(orig, image, Size(), ratio, ratio);
        c.setImage(image);  // change the image during waitKeyEx(0)
        w->setLocation(Point(image.cols - 20, 0)); // keep aligned to top right without a layout manager
    }
    );

    Button::create(c,                               // on the canvas
                   Point(0, 0),                     // top-left aligned on the screen
                   "-",                             // displayed on button
                   "decrease size",                 // displayed during mouse hover
                   [&c, &orig, &image, &ratio, rightSideButton](Widget*) // callback can be a C++11 lambda
    {
        ratio -= 0.05;
        cv::resize(orig, image, Size(), ratio, ratio);
        c.setImage(image);  // change the image during waitKeyEx(0)
        rightSideButton->setLocation(Point(image.cols - 20, 0)); // keep aligned to top right without a layout manager
    }
    );

    int key = 0;
    Mat out;
    do
    {
        c.redrawOn(image, out);  // draw the canvas on the image copy
        c.imshow(out);
        key = c.waitKeyEx(); // GUI and callbacks happen here

    } while (key != 'q');

    destroyAllWindows();

    return 0;
}
