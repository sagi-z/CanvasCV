#include "canvascv/canvas.h"

// These are used to create widgets
#include "canvascv/widgets/text.h"
#include "canvascv/widgets/button.h"
#include "canvascv/widgets/hframe.h"
#include "canvascv/widgets/vframe.h"
#include "canvascv/widgets/widgetfactory.h"

#include <iostream>

#include <opencv2/highgui.hpp>

// This was integrated into c++17, but experimental in c++11,
//  which is currently our minimum version
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

using namespace std;
using namespace cv;
using namespace canvascv;

class FileBrowser : public CompoundWidget
{
public:
    static std::shared_ptr<FileBrowser> create(Layout &layoutVal,
                                               const std::string &startPath,
                                               Widget::CBUserSelection cbUserSelection = Widget::CBUserSelection(),
                                               const cv::Point &pos = cv::Point(-1,-1))
    {
        shared_ptr<FileBrowser> fb = WidgetFactoryT<FileBrowser>::newWidget(layoutVal, pos);
        fb->setCurrentPath(startPath);
        fb->userCB = cbUserSelection;
        return fb;
    }

    string getCurrentPath() const
    {
        return currentPath;
    }

    void setCurrentPath(const string &value)
    {
        if (currentPath != value)
        {
            currentPath = value;
            setDirty();
        }
    }

    virtual const char *getType() const {return type;}
    static const char *type;

    string getUserSelection() const
    {
        return userSelection;
    }

protected:
    friend class canvascv::WidgetFactory;
    template <class T> friend class canvascv::WidgetFactoryT;

    FileBrowser(const cv::Point &pos)
        : CompoundWidget(pos)
    {
        top = VFrame::create(*this, pos).get(); // note the get()
        top->setFrameRelief(RAISED);
        top->setPadding(5);
        pathFrame = HFrame::create(*top).get(); // note the get()
        pathFrame->setFrameRelief(Widget::SELECTED);
        pathFrame->setWrap(true);
        filesFrame = HFrame::create(*top).get(); // note the get()
        filesFrame->setFrameRelief(Widget::SELECTED);
        filesFrame->setWrap(true);
        auto okCancel = HorizontalLayout::create(*top);
        okCancel->setLayoutAnchor(CENTER);
        Button::create(*okCancel, "Cancel", "", [this](Widget*) {rmvFromLayout();});
        Button::create(*okCancel, "Ok", "", [this](Widget*)
        {
            userSelection = currentPath;
            if (userCB) userCB(this, 0);
            rmvFromLayout();
        });
    }

    virtual void recalcCompound()
    {
        fs::path pathObj(getCurrentPath());
        if (fs::is_directory(pathObj) &&
                currentDir != pathObj.string())
        {
            for (auto f : files) { f->rmvFromLayout(); }
            files.clear();
            currentDir = pathObj.string();
            for(auto& p: fs::directory_iterator(getCurrentPath()))
            {
                if (fs::is_directory(p))
                {
                    Button *dir = Button::create(*filesFrame, p.path().filename().string()).get();
                    dir->notifyOnChange([this, p](Widget *, Widget::State state) {
                        if (state == Widget::PRESS) {
                            setCurrentPath(p.path().string());
                        }
                    });
                    files.push_back(dir);
                }
                else
                {
                    Button *file = Button::create(*filesFrame, p.path().filename().string()).get();
                    file->setFlatButton();
                    file->notifyOnChange([this, p](Widget *, Widget::State state) {
                        if (state == Widget::PRESS) {
                            setCurrentPath(p.path().string());
                        }
                    });
                    files.push_back(file);
                }
            }
        }
        for (auto b : path) { b->rmvFromLayout(); }
        path.clear();
        list<fs::path> parts;
        do
        {
            parts.push_front(pathObj);
            pathObj = pathObj.parent_path();
        } while (pathObj.string().length());
        for (auto &p : parts)
        {
            if (fs::is_directory(p))
            {
                Button *dir = Button::create(*pathFrame, p.filename().string()).get();
                dir->notifyOnChange([this, p](Widget *, Widget::State state) {
                    if (state == Widget::PRESS) {
                        setCurrentPath(p.string());
                    }
                });
                path.push_back(dir);
            }
            else
            {
                Button *file = Button::create(*pathFrame, p.filename().string()).get();
                file->setFlatButton();
                path.push_back(file);
            }
        }
        top->update(); // Since we're making widgets dirty in recalc*() we need to manually update them
        recalcRect();
    }

private:
    CBUserSelection userCB;
    fs::path currentDir;
    string currentPath;
    string userSelection;

    // It's enough to keep pointers.
    // The layout is holding the smart pointers,
    //  and that's keeping the reference count up.
    VFrame *top;
    HFrame *pathFrame;
    list<Button*> path;
    HFrame *filesFrame;
    list<Button*> files;
};
const char *FileBrowser::type = "FileBrowser";

REGISTER_WIDGET(FileBrowser);

static Mat readImg(const string &name)
{
    Mat image;
    Mat orig = imread(name, IMREAD_UNCHANGED);
    if (! orig.empty())
    {
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
    return image;
}

int main(int argc, char **argv)
{
    --argc;
    ++argv;
    Mat image;
    if (argc)
    {
        image = readImg(argv[0]);
        if (image.empty())
        {
            Canvas::fatal(string("Cannot load image ") + argv[0], -1);
        }
    }
    else
    {
        Canvas::fatal("Must get a path to an image as a parameter" , -1);
    }

    fs::path startPath(argv[0]);
    startPath = startPath.parent_path();

    Canvas c("Canvas", image.size());

    namedWindow("Canvas", WINDOW_AUTOSIZE);
    c.setMouseCallback(); // optional for mouse usage see also (example_selectbox.cpp)

    shared_ptr<FileBrowser> fb;
    Widget::CBUserSelection cb = [&c, &image](Widget *w, int)
    {
        cout << "CB: Got user selection '" << ((FileBrowser*)w)->getUserSelection() << "'" << endl;
        Mat tmp = readImg(((FileBrowser*)w)->getUserSelection());
        if (tmp.empty())
        {
            cout << "OpenCV cannot read this image" << endl;
        }
        else
        {
            image = tmp;
            c.setSize(image.size());
        }
    };

    int delay = 1000/25; // if using a polling API we need a delay
    int key = 0;
    Mat out; // keeping it out of the loop is a little more efficient
    while (key != 'q')
    {
        if (! fb)
        {
            fb = FileBrowser::create(c, startPath.string(), cb);
            fb->setFillColor(Colors::RoyalBlue);
            fb->setAlpha(128);
        }
        c.redrawOn(image, out);
        imshow("Canvas", out);
        key = c.waitKeyEx(delay); // GUI and callbacks happen here
        if (fb->getUserSelection().length())
        {
            cout << "Loop: Got user selection '" << fb->getUserSelection() << "'" << endl;
            cout << "Starting a new FileBrowser" << endl;
            startPath = fb->getUserSelection();
            if (! fs::is_directory(startPath)) startPath = startPath.parent_path();
            fb = FileBrowser::create(c, startPath.string(), cb);
            fb->setFillColor(Colors::RoyalBlue);
            fb->setAlpha(128);
        }
        if (fb->isRemoved() && ! fb->getUserSelection().length())
        {
            cout << "User canceled" << endl;
            break;
        }
    }

    destroyAllWindows();
    return 0;
}
