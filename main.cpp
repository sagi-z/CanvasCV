#include "canvas.h"

// We don't need these includes for default creation of objects with the mouse.
// We only need them if we want to customize or create shapes on canvas form the code.
#include "linecrossing.h"
#include "textbox.h"
#include "shapesconnector.h"
#include "ellipse.h"
#include "button.h"
#include "verticallayout.h"

#include <iostream>
#include <iterator>

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace canvascv;

static void mouseCB(int event, int x, int y, int flags, void* userData) {
    (void)flags;
    Canvas *pCanvas=reinterpret_cast<Canvas*>(userData);
    switch( event )
    {
    case EVENT_LBUTTONDOWN:
        pCanvas->onMousePress(Point(x,y));
        break;
    case EVENT_LBUTTONUP:
        pCanvas->onMouseRelease(Point(x,y));
        break;
    case EVENT_MOUSEMOVE:
        pCanvas->onMouseMove(Point(x,y));
        break;
    }
}

static string gHelpMsg =
"Usage:\n"
"=====\n"
"1: Line\n"
"2: Arrow\n"
"3: TextBox\n"
"4: LineCrossing\n"
"5: Quadrilateral\n"
"6: Rectangle\n"
"7: Ellipse\n"
"8: ShapesConnector\n"
"9: LabeledShapesConnector\n"
"h: toggle usage message\n"
"*: toggle canvas on/off\n"
"c: clear the canvas\n"
"s: save to config.xml\n"
"l: load from config.xml\n"
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
 *                +------------+
 *                |locked shape|
 *                +------------+
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
    textBox->setText("locked shape");
    ellipse->setRect(RotatedRect(center,
                                 Size2f(40,60), // width and height
                                 0));     // angle in degrees
    Handle *head = *(std::next(ellipse->getConnectionTargets().begin(),3)); // 4th rotation handle
    Handle *tail = *textBox->getConnectionTargets().begin();
    connector->connectHead(*ellipse, *head);
    connector->connectTail(*textBox, *tail);

    // set a specific style for these shapes
    ellipse->setOutlineColor(Colors::BLACK);
    ellipse->setThickness(2);
    textBox->setOutlineColor(Colors::BLACK);
    textBox->setThickness(2);
    textBox->setFontScale(2);
    connector->setOutlineColor(Colors::P1_GRAY);
    connector->setFillColor(Colors::P1_GRAY);

    // Lock them all
    textBox->setLocked(true);
    ellipse->setLocked(true);
    connector->setLocked(true);

    // create 2 invisible widgets
    shared_ptr<FloatingText> floatingText = FloatingText::newFloatingText(c, (*head)(),
                                                                          "These 3 objects are locked.\n"
                                                                          "They are an Ellipse, ShapesConnector and a TextBox.\n"
                                                                          "You can still select them and delete them.",
                                                                          FloatingText::BOTTOM_LEFT);
    floatingText->setVisible(false);

    shared_ptr<VerticalLayout> buttons = VerticalLayout::newVerticalLayout(c, (*head)());
    buttons->setSpacing(10);
    buttons->setVisible(false);
    buttons->addWidget(Button::newButton(c, "shortTxt\n2 lines",
                                         "Hover with mouse (1).\n"
                                         "Press with mouse (1)."));
    buttons->addWidget(Button::newButton(c, "this is a long text",
                                         "Hover with mouse (2).\n"
                                         "Press with mouse (2)."));

    buttons->at(0)->notifyOnChange([](Widget *w, Widget::State state)
    {
        cout << "widget " << w << " at(0) got state " << state << endl;
    });

    buttons->at(1)->notifyOnChange([](Widget *w, Widget::State state)
    {
        cout << "widget " << w << " at(1) got state " << state << endl;
    });

    ellipse->notifyOnEvent([buttons, &c, floatingText](Shape *shape, Shape::CBEvent event)
    {
        cout << "ellipse event is "   << event << endl;
        if (event == Shape::SELECT)
        {
            buttons->setVisible(false);
            floatingText->setVisible(true);
        }
        else if (event == Shape::UNSELECT)
        {
            buttons->setVisible(true);
            floatingText->setVisible(false);
        }
        else if (event == Shape::REMOVED)
        {
            c.rmvWidget(buttons);
            c.rmvWidget(floatingText);
        }
    });
}

int main(int argc, char **argv)
{
    --argc;
    ++argv;
    Mat image;
    if (argc) {
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
    } else {
        image.create(640,480,CV_8UC3);
        image = Scalar::all(255);
    }

    Canvas c(image.size());
    c.setShapeType("Line"); // default shape type for direct GUI creation
    c.enableScreenText();
    c.enableStatusMsg();
    help(c);

    // notifyOnCreate callbacks can be used for anything.
    // here we change default attributes in case of TextBox or LineCrossing
    c.notifyOnShapeCreate([](Shape *shape)
    {
        if (shape->getType() == TextBox::type)
        {
            ((TextBox*)shape)->setFontScale(2);
            shape->setOutlineColor(Colors::P1_LBLUE);
        }
        else if (shape->getType() == LineCrossing::type)
        {
            ((LineCrossing*)shape)->setArrowMagnitude(30);
            ((LineCrossing*)shape)->getTextBox()->setOutlineColor(Colors::BLACK);
            ((LineCrossing*)shape)->getTextBox()->setFontScale(2);
            ((LineCrossing*)shape)->getTextBox()->setThickness(2);
            ((LineCrossing*)shape)->getArrow()->setOutlineColor(Colors::P1_ORANGE);
            ((LineCrossing*)shape)->getArrow()->setThickness(2);
            ((LineCrossing*)shape)->getLine()->setOutlineColor(Colors::P1_GRAY);
            ((LineCrossing*)shape)->getLine()->setThickness(3);
        }
    });

    // modify callback
//    c.notifyOnModify([](Shape *shape){cout << "MODIFY " << shape << "\n" << *shape << endl;});

    // delete callback
//    c.notifyOnDelete([](Shape *shape){cout << "DELETE " << shape << "\n" << *shape << endl;});

    // Creating shapes on the canvas from code is also possible
    createShapesFromCodeExample(c, Point(image.cols / 2, image.rows / 2));

    namedWindow("Canvas", WINDOW_AUTOSIZE);
    setMouseCallback("Canvas", mouseCB, &c);

    int delay = 1000/15;
    int key = 0;
    bool canvasOn = true;
    do {
        switch (key) {
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
            c.setShapeType("Quadrilateral");
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
            canvasOn = ! canvasOn;
            break;
        case 'c':
            c.clear();
            break;
        case 's':
        {
            FileStorage fs("config.xml", FileStorage::WRITE);
            fs << "Canvas" << c;
        }
            break;
        case 'l':
        {
            FileStorage fs("config.xml", FileStorage::READ);
            fs["Canvas"] >> c;
        }
            break;
        case 65535:
            c.deleteActive();
            break;
        }

        Mat out;
        if (canvasOn)
        {
            c.redrawOn(image, out);
        }
        else
        {
            out = image;
        }
        imshow("Canvas", out);
        key = waitKeyEx(delay);
        c.consumeKey(key);
    } while (key != 'q');

    destroyAllWindows();
    return 0;
}
