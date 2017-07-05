#include <canvascv/canvas.h>
#include <canvascv/widgets/button.h>
#include <canvascv/widgets/verticallayout.h>

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

    Mat image = imread(argv[0]);
    if (image.empty())
    {
        Canvas::fatal(string("Cannot load image ") + argv[0], -2);
    }

    namedWindow("LayoutButtons", WINDOW_AUTOSIZE); // disable mouse resize
    Canvas c("LayoutButtons", image.size());
    c.setMouseCallback();
    c.enableStatusMsg();

    auto vLayout = VerticalLayout::create(c, {100,100});

    Button::create(*vLayout,        // on the canvas
                   "First Button",  // displayed on button
                   "one");          // displayed during mouse hover

    Button::create(*vLayout,        // on the canvas
                   "Second Button", // displayed on button
                   "two");          // displayed during mouse hover

    Button::create(*vLayout,        // on the canvas
                   "Third Button",  // displayed on button
                   "three");        // displayed during mouse hover

    vLayout->at<Button>(0)->setStretchX(true);
    vLayout->at<Button>(1)->setStretchX(true);
    vLayout->at<Button>(2)->setStretchX(true);

    int key = 0;
    c.setImage(image);
    do
    {
        key = c.waitKeyEx(); // GUI and callbacks happen here

    } while (key != 'q');

    destroyAllWindows();

    return 0;
}
