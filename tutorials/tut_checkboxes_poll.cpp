#include <canvascv/canvas.h>
#include <canvascv/widgets/checkboxes.h>

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

    Canvas c("CheckBoxes", image.size());

    namedWindow("CheckBoxes", WINDOW_AUTOSIZE);

    c.setMouseCallback(); // optional for mouse usage (see also example_selectbox.cpp)

    auto checkBox = CheckBoxes::create(c, {
                                           "Option1", // index 0
                                           "Option2", // index 1
                                           "Exit"     // index 2
                                       },
                                       Widget::CBUserSelection(), // empty callback
                                       Point(image.cols / 2, image.rows / 2));

    int key = 0;
    int delay = 1000 / 25; // must use a delay when polling
    Mat out;
    do
    {
        c.redrawOn(image, out);
        c.imshow(out);
        key = c.waitKeyEx(delay); // GUI and callbacks happen here
    } while (! checkBox->isChecked(2));

    destroyAllWindows();

    return 0;
}
