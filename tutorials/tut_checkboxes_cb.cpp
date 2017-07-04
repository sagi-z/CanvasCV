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
    c.enableScreenText();
    c.setScreenText("Use q to exit");

    namedWindow("CheckBoxes", WINDOW_AUTOSIZE);

    c.setMouseCallback(); // optional for mouse usage (see also example_selectbox.cpp)

    CheckBoxes::create(c, {
                           "Option1",
                           "Option2",
                           "Option3"
                       },
                       [&c](Widget *w, int)
    {
        CheckBoxes *checkBoxes = (CheckBoxes *) w;
        c.setScreenText(CCV_STR(
                            "Option1:" <<  checkBoxes->isChecked(0) << "\n" <<
                            "Option2:" <<  checkBoxes->isChecked(1) << "\n" <<
                            "Option3:" <<  checkBoxes->isChecked(2) << "\n\n" <<
                            "Use q to exit"
                            ));
    },
    {image.cols / 2, image.rows / 2});

    int key = 0;
    c.setImage(image);
    do
    {
        key = c.waitKeyEx(); // GUI and callbacks happen here
    } while (key != 'q');

    destroyAllWindows();

    return 0;
}
