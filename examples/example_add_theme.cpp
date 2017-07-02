#include "canvascv/canvas.h"

// These are used to create widgets
#include "canvascv/widgets/radiobuttons.h"
#include "canvascv/widgets/text.h"
#include "canvascv/widgets/button.h"
#include "canvascv/widgets/hframe.h"
#include "canvascv/themes/themerepository.h"
#include "canvascv/themes/twocoloredtheme.h"

#include <iostream>
#include <iterator>

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace canvascv;

class MyTheme : public TwoColoredTheme
{
public:
    MyTheme(Scalar fgColor, Scalar bgColor, int widthVal)
        : TwoColoredTheme(fgColor, bgColor),
          width(widthVal)
    {}

    virtual void allocateBG(Mat &dst, const Size &size, const Scalar &color)
    {
        TwoColoredTheme::allocateBG(dst, size, color);
        if (! dst.empty())
        {
            Point pFrom(0,0);
            Point pTo(0,size.height);
            int spacing = width * 2;
            for (pFrom.x = 0, pTo.x = pFrom.x - size.height ; pTo.x < size.width; pFrom.x += spacing, pTo.x += spacing)
            {
                cv::line(dst, pFrom, pTo, color*1.5, width, LINE_AA);
            }
        }
    }
private:
    int width;
};

int main(int argc, char **argv)
{
    --argc;
    ++argv;
    Mat image;
    if (argc)
    {
        Mat orig = imread(argv[0]);
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
        Canvas::fatal("Must get a path to an image as a parameter" , -1);
    }

    Canvas c("Canvas", image.size());
    auto layout = HFrame::create(c, Point(5, image.rows / 2.));
    Widget::CBUserSelection cb = [&c, &layout](Widget *w, int index) {
        RadioButtons *rb = (RadioButtons*)w;
        string themeName = rb->getTextAt(index);
        ThemeRepository::setCurrentTheme(themeName);
        c.applyTheme();
        layout->at<Text>(0)->setText(CCV_STR("User selected theme '" << index << "': '" << themeName << "'\n"));
    };

    ThemeRepository::addTheme("MyTheme1", new MyTheme(Colors::Red, Colors::Pink, 10));
    ThemeRepository::addTheme("MyTheme2", new MyTheme(Colors::Red, Colors::Pink, 20));
    ThemeRepository::addTheme("MyTheme3", new MyTheme(Colors::Blue, Colors::Green, 20));
    vector<string> availThemes = ThemeRepository::availThemes();

    auto iter = std::find(availThemes.begin(), availThemes.end(), ThemeRepository::getCurrentThemeName());
    int i = std::distance(availThemes.begin(), iter);

    auto text = Text::create(*layout, "", Widget::TOP, Widget::CENTER);
    text->setMaxWidth(100);
    text->setAlpha(0);
    RadioButtons::create(*layout, availThemes, i, cb);
    Button::create(*layout, "First button")->setLayoutAnchor(Widget::CENTER);
    Button::create(*layout, "Second button")->setLayoutAnchor(Widget::CENTER);

    namedWindow("Canvas", WINDOW_AUTOSIZE);
    c.setMouseCallback(); // optional for mouse usage see also (example_selectbox.cpp)

    int key = 0;
    Mat out; // keeping it out of the loop is a little more efficient
    while (key != 'q')
    {
        c.redrawOn(image, out);
        imshow("Canvas", out);
        key = c.waitKeyEx(); // GUI and callbacks happen here
    }

    destroyAllWindows();
    return 0;
}


