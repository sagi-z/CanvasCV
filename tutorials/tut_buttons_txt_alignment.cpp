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

    Mat image = imread(argv[0]);
    if (image.empty())
    {
        Canvas::fatal(string("Cannot load image ") + argv[0], -2);
    }

    namedWindow("ButTextAlign", WINDOW_AUTOSIZE); // disable mouse resize
    Canvas c("ButTextAlign", image.size());
    c.enableStatusMsg(); // shows hover over button msgs
    c.setMouseCallback();

    string buttonText = "short\n"
                        "a longer line";
    auto b1 = Button::create(c,               // on the canvas
                             Point(10,10),    // top-right aligned on the screen
                             buttonText,      // displayed on button
                             "centered");     // displayed during mouse hover
    b1->setFlowAnchor(Widget::CENTER);

    auto b2 = Button::create(c,               // on the canvas
                             Point(10,110),    // top-right aligned on the screen
                             buttonText,      // displayed on button
                             "left aligned"); // displayed during mouse hover
    b2->setFlowAnchor(Widget::LEFT);

    auto b3 = Button::create(c,               // on the canvas
                             Point(10,210),    // top-right aligned on the screen
                             buttonText,      // displayed on button
                             "right aligned");// displayed during mouse hover
    b3->setFlowAnchor(Widget::RIGHT);

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
