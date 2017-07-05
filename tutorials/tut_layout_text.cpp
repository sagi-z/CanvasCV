#include <canvascv/canvas.h>
#include <canvascv/widgets/text.h>
#include <canvascv/widgets/verticallayout.h>
#include <canvascv/widgets/horizontallayout.h>

using namespace canvascv;

void help(Canvas &c)
{
    static bool showHelp = true;
    static string helpMsg =
            "Usage:\n"
            "=====\n"
            "h: toggle usage message\n"
            "*: toggle canvas on/off\n"
            "q: exit";


    if (showHelp) c.setScreenText(helpMsg);
    else c.setScreenText("");

    showHelp = ! showHelp;
}

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

    Canvas c("LayoutTxt", image.size());
    c.enableScreenText(); // see it's documentation

    help(c);

    namedWindow("LayoutTxt", WINDOW_AUTOSIZE); // disable mouse resize

    auto vLayout = VerticalLayout::create(c);
    vLayout->setStretchXToParent(true);
    vLayout->setStretchYToParent(true);

    auto hLayout = HorizontalLayout::create(*vLayout);
    hLayout->setLayoutAnchor(Widget::CENTER);
    hLayout->setStretchYToParent(true);

    auto txt = Text::create(*hLayout, "Target Acquired!");
    txt->setLayoutAnchor(Widget::CENTER);
    txt->setFontHeight(50);
    txt->setOutlineColor(Colors::Red);
    txt->setThickness(2);

    int key = 0;
    Mat out;
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

        c.redrawOn(image, out);  // draw the canvas on the image copy

        imshow("LayoutTxt", out);

        key = c.waitKeyEx(); // GUI and callbacks happen here

    } while (key != 'q');

    destroyAllWindows();

    return 0;
}
