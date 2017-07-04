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

    Widget::CBUserSelection  cb;
    int cnt = 0;
    cb = [&c, &cb, &cnt](Widget *w, int index)
    {
        MsgBox *pMsgBox = (MsgBox*) w;
        c.setScreenText(CCV_STR( "User pressed option numer '" << index << "'\n" <<
                                 "The button text is '" << pMsgBox->getTextAt(index) << "'\n\n" <<
                                 "(press q to exit)"));
        MsgBox::create(c, CCV_STR("Do you really want to do that? " << "(" << ++cnt << ")"), {
                           "Yes", "No"
                       }, cb);
    };
    MsgBox::create(c, "Do you really want to do that?", {"Yes", "No"}, cb);

    int key = 0;
    Mat out;
    do
    {
        c.redrawOn(image, out);  // draw the canvas on the image copy

        imshow("MsgBox example", out);

        key = c.waitKeyEx(); // GUI and callbacks happen here
    } while (key != 'q');

    destroyAllWindows();

    return 0;
}
