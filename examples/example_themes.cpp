#include "canvas.h"

// **Optional
// These are used to create widgets
#include "widgets/radiobuttons.h"
#include "themes/themerepository.h"
#include "themes/twocoloredtheme.h"

#include <iostream>
#include <iterator>

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace canvascv;

int main(int argc, char **argv)
{
    --argc;
    ++argv;
    Mat image;
    if (argc)
    {
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
    }
    else
    {
        cout << "Must get a path to an image as a parameter" << endl;
        return -1;
    }

    Canvas c(image.size());
    c.enableScreenText();
    Widget::CBUserSelection cb = [&c](Widget *w, int index) {
        stringstream s;
        RadioButtons *rb = (RadioButtons*)w;
        string themeName = rb->getTextAt(index);
        s << "User selected theme '" << index << "': '" << themeName << "'\n";
        ThemeRepository::setCurrentTheme(themeName);
        c.applyTheme();
        c.setScreenText(s.str());
    };

    vector<string> availThemes = ThemeRepository::availThemes();
    auto iter = std::find(availThemes.begin(), availThemes.end(), ThemeRepository::getCurrentThemeName());
    int i = std::distance(availThemes.begin(), iter);
    auto radioButtons = RadioButtons::create(c, availThemes, i, cb,
                                             Point(image.cols / 2., image.rows / 2.));

    namedWindow("Canvas", WINDOW_AUTOSIZE);
    c.setMouseCallback("Canvas"); // optional for mouse usage see also (example_selectbox.cpp)

    int delay = 1000/25;
    int key = 0;
    Mat out; // keeping it out of the loop is a little more efficient
    while (radioButtons->getSelection() != 3 && key != 'q')
    {
        c.redrawOn(image, out);
        imshow("Canvas", out);
        key = waitKeyEx(delay); // GUI and callbacks happen here
    }

    destroyAllWindows();
    return 0;
}
