#include <canvascv/canvas.h>
#include <canvascv/shapes/polygon.h>

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace canvascv;

void handlePolyPoints(Canvas &c, Polygon &poly)
{
    vector<Point> polyPoints;
    poly.getPoints(polyPoints);
    stringstream s;
    s <<  "polygon points are at :\n[";
    for (auto &v : polyPoints)
    {
        s << " " << v << " ";
    }
    s <<  "]\n\n";
    s <<  "Use q to exit.\n";
    s <<  "Use s to save.\n";
    s <<  "Use l to load.";
    c.setScreenText(s.str());
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

    Canvas c("Persistent Shapes", image.size());
    c.enableScreenText();

    c.enableStatusMsg();
    c.setDefaultStatusMsg("Click-Release-Drag to create a single polygon.\n"
                          "Select/Unselect with the mouse.\n"
                          "Drag selected polygon with the mouse.\n"
                          "DEL deleted the selected shape.");

    c.setShapeType(Polygon::type); // default shape type for direct GUI creation
    c.notifyOnShapeCreate([&c](Shape* shape)
    {
        handlePolyPoints(c, *(Polygon*) shape); // it can only be a Polygon
        c.setShapeType(""); // default shape type for direct GUI creation
    });
    c.notifyOnShapeModify([&c](Shape* shape)
    {
        handlePolyPoints(c, *(Polygon*) shape);
    });
    c.notifyOnShapeDelete([&c](Shape*)
    {
        c.setShapeType(Polygon::type); // default shape type for direct GUI creation
    });

    namedWindow("Persistent Shapes", WINDOW_AUTOSIZE);
    c.setMouseCallback(); // optional for mouse usage see also (example_selectbox.cpp)

    int key = 0;
    Mat out; // keeping it out of the loop is a little more efficient
    do
    {
        switch (key)
        {
        case 's':
            c.writeShapesToFile("canvas.xml");
            break;
        case 'l':
            c.readShapesFromFile("canvas.xml");
            break;
        case 65535:
            c.deleteActive();
            c.setScreenText("");
            break;
        }

        c.redrawOn(image, out);
        c.imshow(out);
        key = c.waitKeyEx(); // GUI and callbacks happen here
    } while (key != 'q');

    destroyAllWindows();
    return 0;
}
