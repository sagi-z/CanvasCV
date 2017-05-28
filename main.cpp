#include "canvas.h"

#include <iostream>

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace canvasvc;

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

void help()
{
    cout << "Usage:\n" <<
            "=====\n" <<
            "1: Line\n" <<
            "2: Arrow\n" <<
            "3: TextBox\n" <<
            "4: LineCrossing\n" <<
            "5: Quadrilateral\n" <<
            "6: Rectangle\n" <<
            "7: Ellipse\n" <<
            "8: ShapesConnector\n" <<
            "9: LabeledShapesConnector\n" <<
            "h: print usage message\n" <<
            "c: clear the canvas\n" <<
            "s: save to config.xml\n" <<
            "l: load from config.xml\n" <<
            "d: delete active shape\n" <<
            "q: exit" << endl;
}

int main(int argc, char **argv)
{
    --argc;
    ++argv;
    Mat image;
    if (argc) {
        image = imread(argv[0]);
        if (image.empty()) {
            cerr << "Cannot load image " << argv[0] << endl;
            return -1;
        }
    } else {
        image.create(640,480,CV_8UC3);
        image = Scalar::all(255);
    }

    help();

    Canvas c;
    c.setShapeType("Line");
//    c.notifyOnCreate([](Shape *shape){cout << "CREATE " << shape << "\n" << *shape << endl;});
//    c.notifyOnModify([](Shape *shape){cout << "MODIFY " << shape << "\n" << *shape << endl;});
//    c.notifyOnDelete([](Shape *shape){cout << "DELETE " << shape << "\n" << *shape << endl;});

    namedWindow("Canvas", WINDOW_AUTOSIZE);
    setMouseCallback("Canvas", mouseCB, &c);

    int delay = 1000/24;
    int key = 0;
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
            help();
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
        case 'd':
            c.deleteActive();
            break;
        }

        Mat out;
        c.redrawOn(image, out);
        imshow("Canvas", out);
        key = waitKey(delay);
        c.consumeKey(key);
    } while (key != 'q');

    destroyAllWindows();
    return 0;
}
