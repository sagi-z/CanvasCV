Creating shapes {#tutshapes}
===============

CanvasCV's support of shapes is one of it's strongest points.

You have 2 ways of creating shapes:
* **From the GUI** (with the mouse and keyboard) - for example to let the
user select a polygon mask as an area of intereset for the CV.
* **From the code** - for example creating a clickable rectangle on screen
to which a *SelectionBox* widget can be applied.

[TOC]

# Creating a polygon from the GUI {#shapes_s1}
In this example we want to user to mark a polygon area on the image or
frame.

There could be various reasons for that:
* Maybe the camera is capturing a large area and we want to operate
our algorithm on a relevant dynamic subset of it.
* The user selects a polygon area that triggers an alert when tracked
objects are inside it.
* etc.

When the user creates/modifies/deletes the polygon we'll just print the
vertices to the screen.

Here is the code:
~~~~~~~{.cpp}
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
    s <<  "Use q to exit.";
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

    Canvas c("Shapes", image.size());
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

    namedWindow("Shapes", WINDOW_AUTOSIZE);
    c.setMouseCallback(); // optional for mouse usage see also (example_selectbox.cpp)

    int key = 0;
    Mat out; // keeping it out of the loop is a little more efficient
    do
    {
        switch (key)
        {
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
~~~~~~~
Notes:
* canvascv::Canvas::enableStatusMsg() is optional, but recommended when
using shapes and widgets. It takes a small portion of the screen area,
but gives helpful information to the user.
* canvascv::Canvas::setDefaultStatusMsg() is optional. There are built
in status messages that are displayed during shape creation and editing.
When nothing is selected, then your default status message will be
displayed.
* The Canvas needs to know what shape to create when the user left
clicks with the mouse. This is done with
canvascv::Canvas::setShapeType().
* You can register for Canvas create/modify/delete notification on
the Shapes that it creates/modify/deletes. This is the place to react to
the user's GUI actions.
* When executed with a path to an image, this gives you (depends on your image):
@image html tut_shapes_gui.jpg
<BR>

# Creating a clickable rectangle from code {#shapes_s2}

As you know, in CV tracking is split into 2 phases - detection and
tracking.

Let's assume we have a people detection code, and we're detecting people
in a croud.

We want to enable the user to choose a specific detection to start
tracking. In the most simple form - we want the rectangles we draw to do
something when they are clicked.

In this tutorial we won't track anything, but just output the chosen
rectangle coordinates to the screen.

The base code we'll be using is from *OpenCV-3.2.0/samples/cpp/peopledetect.cpp*.

The modified code can be optimized for performance, but here we just
want to show the *CanvasCV* way of doing it.

You are encouraged to diff the tutorial version and the OpenCV version
with your favorite diff tool, but the most important difference is this:

~~~~~~~{.diff}
<         rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
---
>         shared_ptr<Rectangle> shapeRect = canvas.createShape<Rectangle>();
>         shapeRect->setRect(cv::RotatedRect(Point(r.x + r.width / 2., r.y + r.height / 2.), r.size(), 0));
>         shapeRect->setOutlineColor(Colors::Green);
>         shapeRect->setThickness(3);
>         shapeRect->setLocked(true); // cannot be dragged
>         shapeRect->notifyOnEvent([&canvas](Shape *shape, Shape::Event evt)
>         {
>             if (evt == Shape::SELECT)
>             {
>                 canvas.setScreenText(CCV_STR("User clicked in rect " << ((Rectangle*)shape)->getRect().boundingRect()));
>             }
>         });
~~~~~~~

In the code above, instead of drawing a rectangle, we create a
canvascv::Rectangle object on the *Canvas* and attach a callback to that
specific instance.

The canvascv::Shape::Event we want is *SELECT*, which is when a shape is
selected with the mouse (Clicked).

The full code is a little long because the original code is long:

~~~~~~~{.cpp}
#include <iostream>
#include <stdexcept>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
using namespace std;

// CanvasCV change#1 - include and namespace
#include <canvascv/canvas.h>
#include <canvascv/shapes/rectangle.h>
using namespace canvascv;


const char* keys =
{
    "{ help h      |                     | print help message }"
    "{ image i     |                     | specify input image}"
    "{ camera c    |                     | enable camera capturing }"
    "{ video v     | ../data/vtest.avi   | use video as input }"
    "{ directory d |                     | images directory}"
};

// CanvasCV change#8 - we draw on the canvas instead of the real img
static void detectAndDraw(const HOGDescriptor &hog, Mat &img, Canvas &canvas)
{
    vector<Rect> found, found_filtered;
    double t = (double) getTickCount();
    // Run the detector with default parameters. to get a higher hit-rate
    // (and more false alarms, respectively), decrease the hitThreshold and
    // groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
    hog.detectMultiScale(img, found, 0, Size(8,8), Size(32,32), 1.05, 2);
    t = (double) getTickCount() - t;
    cout << "detection time = " << (t*1000./cv::getTickFrequency()) << " ms" << endl;

    for(size_t i = 0; i < found.size(); i++ )
    {
        Rect r = found[i];

        size_t j;
        // Do not add small detections inside a bigger detection.
        for ( j = 0; j < found.size(); j++ )
            if ( j != i && (r & found[j]) == r )
                break;

        if ( j == found.size() )
            found_filtered.push_back(r);
    }

    for (size_t i = 0; i < found_filtered.size(); i++)
    {
        Rect r = found_filtered[i];

        // The HOG detector returns slightly larger rectangles than the real objects,
        // so we slightly shrink the rectangles to get a nicer output.
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
        shared_ptr<Rectangle> shapeRect = canvas.createShape<Rectangle>();
        shapeRect->setRect(cv::RotatedRect(Point(r.x + r.width / 2., r.y + r.height / 2.), r.size(), 0));
        shapeRect->setOutlineColor(Colors::Green);
        shapeRect->setThickness(3);
        shapeRect->setLocked(true); // cannot be dragged
        shapeRect->notifyOnEvent([&canvas](Shape *shape, Shape::Event evt)
        {
            if (evt == Shape::SELECT)
            {
                canvas.setScreenText(CCV_STR("User clicked in rect " << ((Rectangle*)shape)->getRect().boundingRect()));
            }
        });
    }
}

int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, keys);

    if (parser.has("help"))
    {
        cout << "\nThis program demonstrates the use of the HoG descriptor using\n"
            " HOGDescriptor::hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());\n";
        parser.printMessage();
        cout << "During execution:\n\tHit q or ESC key to quit.\n"
            "\tUsing OpenCV version " << CV_VERSION << "\n"
            "Note: camera device number must be different from -1.\n" << endl;
        return 0;
    }

    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    namedWindow("people detector", 1);

    // CanvasCV change#2 - attach a canvas to a window with mouse callbacks
    Canvas canvas("people detector");
    canvas.enableScreenText();
    canvas.setMouseCallback();

    string pattern_glob = "";
    string video_filename = "../data/vtest.avi";
    int camera_id = -1;
    if (parser.has("directory"))
    {
        pattern_glob = parser.get<string>("directory");
    }
    else if (parser.has("image"))
    {
        pattern_glob = parser.get<string>("image");
    }
    else if (parser.has("camera"))
    {
        camera_id = parser.get<int>("camera");
    }
    else if (parser.has("video"))
    {
        video_filename = parser.get<string>("video");
    }

    if (!pattern_glob.empty() || camera_id != -1 || !video_filename.empty())
    {
        //Read from input image files
        vector<String> filenames;
        //Read from video file
        VideoCapture vc;
        Mat frame;

        if (!pattern_glob.empty())
        {
            String folder(pattern_glob);
            glob(folder, filenames);
        }
        else if (camera_id != -1)
        {
            vc.open(camera_id);
            if (!vc.isOpened())
            {
                // CanvasCV change#3 - using fatal
                Canvas::fatal(CCV_STR("can't open camera: " << camera_id), -1);
            }
        }
        else
        {
            vc.open(video_filename.c_str());
            // CanvasCV change#4 - using fatal
            if (!vc.isOpened())
                Canvas::fatal(CCV_STR("can't open video file: " << video_filename), -1);
        }

        vector<String>::const_iterator it_image = filenames.begin();

        Mat out;
        for (;;)
        {
            if (!pattern_glob.empty())
            {
                bool read_image_ok = false;
                for (; it_image != filenames.end(); ++it_image)
                {
                    cout << "\nRead: " << *it_image << endl;
                    // Read current image
                    frame = imread(*it_image);

                    if (!frame.empty())
                    {
                        ++it_image;
                        read_image_ok = true;
                        break;
                    }
                }

                //No more valid images
                if (!read_image_ok)
                {
                    //Release the image in order to exit the while loop
                    frame.release();
                }
            }
            else
            {
                vc >> frame;
            }

            if (frame.empty())
                break;

            // CanvasCV change#5 - clean & set canvas limits
            canvas.clearShapes();
            canvas.setSize(frame.size());

            detectAndDraw(hog, frame, canvas);

            // CanvasCV change#6 - draw shapes & widgets
            canvas.redrawOn(frame, out);
            canvas.imshow(out);

            // CanvasCV change#7 - get keyboard events from the canvas
            int c = canvas.waitKeyEx( vc.isOpened() ? 30 : 0 ) & 255;
            if ( c == 'q' || c == 'Q' || c == 27)
                break;
        }
    }

    return 0;
}
~~~~~~~
Notes for the `//CanvasCV change#...` comments above:
* Here the canvas is cleared at each frame. There are ways to optimize
  this, but first you'll need to see if this affects your performance at
  all.
* As a general rule, if the image size changes, then use
  canvascv::Canvas::setSize() to adapt to the new size.
* When using the CanvasCV, you create shapes and widgets on the
  canvascv::Canvas and not on the *Mat*.
* canvascv::Canvas::waitKeyEx() knows to update it's internal shapes and
  widgets even if you pass 0 as a blocking delay indicator.
* *CCV_STR* lets you create a string as you would write into a stream.
* When executed with --video="Path to opencv-3.2.0/samples/data/vtest.avi", you can get
@image html tut_shapes_code.jpg
<BR>

**That's all for this tutorial**
