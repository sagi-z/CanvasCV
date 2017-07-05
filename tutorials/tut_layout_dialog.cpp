#include <canvascv/canvas.h>
#include <canvascv/widgets/button.h>
#include <canvascv/widgets/hframe.h>
#include <canvascv/widgets/vframe.h>
#include <canvascv/widgets/radiobuttons.h>
#include <canvascv/widgets/checkboxes.h>
#include <canvascv/widgets/text.h>

using namespace cv;
using namespace canvascv;

void buildDemoDialog(Canvas &c)
{
    auto topFrame = VFrame::create(c, {100,100});
    auto titleFrame = VFrame::create(*topFrame);
    auto bodyFrame = HFrame::create(*topFrame);
    auto leftBodyFrame = VFrame::create(*bodyFrame);
    auto rightBodyFrame = VFrame::create(*bodyFrame);
    auto textInfoFrame = HFrame::create(*rightBodyFrame);
    auto buttonsLayout = HorizontalLayout::create(*rightBodyFrame);

    leftBodyFrame->setFrameRelief(Widget::SUNKEN);
    textInfoFrame->setFrameRelief(Widget::SUNKEN);

    // The title frame and body frame adapt to top frame width
    titleFrame->setStretchX(true);
    bodyFrame->setStretchX(true);

    rightBodyFrame->setStretchY(true);

    // Adjust to titleFrame width and put the text at the center
    auto title = Text::create(*titleFrame, "Dialog title goes here", Widget::CENTER);
    title->setStretchXToParent(true);
    title->setAlpha(0);
    titleFrame->setFrameRelief(Widget::RAISED);

    shared_ptr<Text> textInfo = Text::create(*textInfoFrame, "");
    shared_ptr<RadioButtons> radioButtons = RadioButtons::create(*leftBodyFrame, {"RBOption1", "RBOption2"}, 0);
    shared_ptr<CheckBoxes> checkBoxes = CheckBoxes::create(*leftBodyFrame, {"CBOption1", "CBOption2", "CBOption3"});

    Widget::CBUserSelection cb = [textInfo, radioButtons, checkBoxes](Widget*,int)
    {
        stringstream s;
        for (int i = 0; i < checkBoxes->size(); ++i)
            s << "CheckBox option '" << checkBoxes->getTextAt(i) << "' is " << checkBoxes->isChecked(i) << "\n";
        s << "RadioButtons selection is '" << radioButtons->getTextAt(radioButtons->getSelection())<< "'\n";
        textInfo->setText(s.str());
    };

    cb(0,0); // manually initialize the textInfo;
    textInfo->setAlpha(0);

    radioButtons->setUserCB(cb);
    checkBoxes->setUserCB(cb);

    Button::create(*buttonsLayout, "Reset")->onPress([radioButtons, checkBoxes](Widget *)
    {
        for (int i = 0; i < checkBoxes->size(); ++i)
            checkBoxes->setChecked(i, false);
        radioButtons->setSelection(0);
    });
    Button::create(*buttonsLayout, "Cancel");
    Button::create(*buttonsLayout, "Ok");
    buttonsLayout->setStretchYToParent(true);
    buttonsLayout->setLayoutAnchor(Widget::CENTER);
    buttonsLayout->doForAll([](Widget *w)
    {
        w->setStretchX(true);
        w->setLayoutAnchor(Widget::BOTTOM);
    }, 1, false);
}

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
        Canvas::fatal("Must get a path to an image as a parameter" , -2);
    }

    namedWindow("Dialog", WINDOW_AUTOSIZE); // disable mouse resize
    moveWindow("Dialog", 10, 10);
    Canvas c("Dialog", image.size());
    c.setMouseCallback();
    c.enableStatusMsg();

    buildDemoDialog(c);

    int key = 0;
    c.setImage(image);
    do
    {
        key = c.waitKeyEx(); // GUI and callbacks happen here

    } while (key != 'q');

    destroyAllWindows();

    return 0;
}
