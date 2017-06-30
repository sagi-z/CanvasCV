// Mandator include file
#include "canvascv/canvas.h"

// We don't need these includes for shape based interaction with the user.
// We need these if we want to customize or create shapes on canvas form the code.
#include "canvascv/shapes/linecrossing.h"
#include "canvascv/shapes/textbox.h"
#include "canvascv/shapes/shapesconnector.h"
#include "canvascv/shapes/ellipse.h"

// These are used to create widgets
#include "canvascv/widgets/button.h"
#include "canvascv/widgets/vframe.h"
#include "canvascv/widgets/hframe.h"

#include <iostream>
#include <iterator>

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace canvascv;

static string gHelpMsg =
"Usage:\n"
"=====\n"
"Use the mouse and keyboard.\n"
"Hover over and click on widgets and shapes.\n"
"Select/Unselect the center Ellipse.\n"
"Use these keys:\n"
"1: Line\n"
"2: Arrow\n"
"3: TextBox\n"
"4: LineCrossing\n"
"5: Polygon\n"
"6: Rectangle\n"
"7: Ellipse\n"
"8: ShapesConnector\n"
"9: LabeledShapesConnector\n"
"h: toggle usage message\n"
"*: toggle canvas on/off\n"
"c: clear the shapes from the canvas\n"
"s: save shapes to config.xml\n"
"l: load shapes from config.xml\n"
"DEL: delete active shape\n"
"q: exit";

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

/*
 * Create a selectable ellipse with a text-box like this:
 *
 *                +--------------+
 *                |locked ellipse|
 *                +--------------+
 *               /
 *              /
 *             O
 *
 */
static void createShapesFromCodeExample(Canvas &c, Point center)
{
    // Add some shapes to the canvas as an example.
    shared_ptr<TextBox> textBox = c.createShape<TextBox>();
    shared_ptr<ShapesConnector> connector = c.createShape<ShapesConnector>();
    shared_ptr<Ellipse> ellipse = c.createShape<Ellipse>();

    textBox->setTL({int(center.x * 1.5), int(center.y * 0.5)});
    textBox->setText("locked ellipse");
    ellipse->setRect(RotatedRect(center,
                                 Size2f(40,60), // width and height
                                 0));     // angle in degrees
    Handle *head = *(std::next(ellipse->getConnectionTargets().begin(),3)); // 4th rotation handle
    Handle *tail = *textBox->getConnectionTargets().begin();
    connector->connectHead(*ellipse, *head);
    connector->connectTail(*textBox, *tail);

    // Lock just the ellipse
    ellipse->setLocked(true);

    // create some widgets
    auto msgs = HFrame::create(c, (*tail)());
    msgs->setFrameRelief(Widget::SUNKEN);
    msgs->setFlowAnchor(Widget::BOTTOM_RIGHT);
    msgs->setVisible(false);

    auto buttons = VFrame::create(c, (*head)());

    Text::create(*msgs,
                 "aligned to top",
                 Widget::BOTTOM)->setLayoutAnchor(Widget::TOP);
    Text::create(*msgs,
                 "aligned to center",
                 Widget::BOTTOM)->setLayoutAnchor(Widget::CENTER);
    Text::create(*msgs,
                 "These 3 objects where precreated.\n"
                 "They are an Ellipse,\n"
                 " ShapesConnector and a TextBox.\n"
                 "You select/edit/move/delete them.\n"
                 "The Ellipse is locked.",
                 Widget::BOTTOM);

    Button::create(*buttons, "right\naligned",
                   "button 1\n"
                   "Hover with mouse.\n"
                   "Press with mouse.");
    Button::create(*buttons, "centered",
                   "button 2\n"
                   "Hover with mouse.\n"
                   "Press with mouse.");
    Button::create(*buttons, "this is a long text",
                   "button 3\n"
                   "Hover with mouse.\n"
                   "Press with mouse.");

    buttons->at(0)->setLayoutAnchor(Widget::RIGHT);
    buttons->at(0)->notifyOnChange([](Widget *w, Widget::State state)
    {
        cout << "widget " << w << " at(0) got state " << state << endl;
    });

    buttons->at(1)->setLayoutAnchor(Widget::CENTER);
    buttons->at(1)->notifyOnChange([](Widget *w, Widget::State state)
    {
        cout << "widget " << w << " at(1) got state " << state << endl;
    });

    ellipse->notifyOnEvent([buttons, msgs, &c](Shape *shape, Shape::CBEvent event)
    {
        cout << "ellipse event is "   << event << endl;
        if (event == Shape::SELECT)
        {
            buttons->setVisible(false);
            msgs->setVisible(true);
        }
        else if (event == Shape::UNSELECT)
        {
            buttons->setVisible(true);
            msgs->setVisible(false);
        }
        else if (event == Shape::REMOVED)
        {
            c.rmvWidget(buttons);
            c.rmvWidget(msgs);
        }
    });
}

int main(int argc, char **argv)
{
    --argc;
    ++argv;
    Mat image;
    if (argc)
    {
        Mat orig = imread(argv[0], IMREAD_UNCHANGED);
        if (orig.empty())
        {
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
        image.create(600, 800, CV_8UC3);
        image = Colors::White;
    }

    cout << "image number of channels is " << image.channels() << endl;
    Canvas c("Canvas", image.size());
    c.setShapeType("Line"); // default shape type for direct GUI creation
    c.enableScreenText();
    c.enableStatusMsg();
    help(c);

    // notifyOnCreate callbacks can be used for anything.
    // here we overide the theme settings in case of LineCrossing
    c.notifyOnShapeCreate([](Shape *shape)
    {
        if (shape->getType() == LineCrossing::type)
        {
            ((LineCrossing*)shape)->setArrowMagnitude(50);
        }
    });

    // Creating shapes on the canvas from code is also possible
    createShapesFromCodeExample(c, Point(image.cols / 2, image.rows / 2));

    namedWindow("Canvas", WINDOW_AUTOSIZE);
    c.setMouseCallback(); // optional for mouse usage see also (example_selectbox.cpp)


    int delay = 1000/25;
    int key = 0;
    Mat out; // keeping it out of the loop is a little more efficient
    do
    {
        switch (key)
        {
        case '1':
            c.setShapeType("Line");
            break;
        case '2':
            c.setShapeType("Arrow");
            break;
        case '3':
            c.setShapeType("TextBox");
            break;
        case '4':
            c.setShapeType("LineCrossing");
            break;
        case '5':
            c.setShapeType("Polygon");
            break;
        case '6':
            c.setShapeType("Rectangle");
            break;
        case '7':
            c.setShapeType("Ellipse");
            break;
        case '8':
            c.setShapeType("ShapesConnector");
            break;
        case '9':
            c.setShapeType("LabeledShapesConnector");
            break;
        case 'h':
            help(c);
            break;
        case '*':
            c.setOn(! c.getOn());
            break;
        case 'c':
            c.clearShapes();
            break;
        case 's':
        {
            c.writeShapesToFile("config.xml");
        }
            break;
        case 'l':
        {
            c.readShapesFromFile("config.xml");
        }
            break;
        case 65535:
            c.deleteActive();
            break;
        }

        c.redrawOn(image, out);
        imshow("Canvas", out);
        key = c.waitKeyEx(delay); // GUI and callbacks happen here
    } while (key != 'q');

    destroyAllWindows();
    return 0;
}
