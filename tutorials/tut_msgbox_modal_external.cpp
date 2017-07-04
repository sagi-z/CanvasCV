#include <canvascv/canvas.h>
#include <canvascv/widgets/msgbox.h>

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

    Canvas c("MsgBox example", image.size());
    c.enableScreenText();

    namedWindow("MsgBox example", WINDOW_AUTOSIZE);

    c.setMouseCallback(); // optional for mouse usage (see also example_selectbox.cpp)

    Mat out;
    int userSelection = 0;
    do
    {
        c.redrawOn(image, out);  // draw the canvas on the image copy

        imshow("MsgBox example", out);

        // the blocking API handles GUI internally
        userSelection = MsgBox::createModal("Modal MsgBox", "Notice this window", {"Ok", "Whatever"});
        c.setScreenText(CCV_STR("User pressed button with index '" << userSelection << "'\n\n" <<
                                "(Choose 'Whatever' to exit)"));
    } while (userSelection == 0);

    destroyAllWindows();

    return 0;
}
