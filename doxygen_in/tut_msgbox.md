Using the MsgBox {#tutmsgbox}
----------------
The canvascv::MsgBox is a little different from other widgets, because it is very high level.

This makes it easy to use and a good starting example for widgets that get mouse input.

The MsgBox is a "one shot user pressed and widget died" kind of widget.

[TOC]

@section tmbsec1 Remember the main loop
Just to remind you from the previous tutotial, the main loop looks something like this:
~~~~~~~{.cpp}
#include <canvascv/canvas.h>
#include <canvascv/widgets/msgbox.h>

using namespace canvascv;

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

    Canvas c("MsgBox example", image.size());
    c.enableScreenText();

    namedWindow("MsgBox example", WINDOW_AUTOSIZE);

    int key = 0;
    Mat out;
    do
    {
        c.redrawOn(image, out);  // draw the canvas on the image copy

        imshow("MsgBox example", out);

        key = c.waitKeyEx(); // GUI and callbacks happen here
    } while (key != 'q');

    destroyAllWindows();

    return 0;
}
~~~~~~~

We want to work on top of an existing image so you can see the transparency.

Let's add the real code now.

@section tmbsec2 A simple modal MsgBox
Since a MsgBox usually requires immediate user attention, you have a way to block other GUI while waiting for a user reaction.

Remeber thah the MsgBox is a "one shot user pressed and widget died" kind of widget. Here we're recreating the MsgBox
each time it dies.

By default a MsgBox will be placed at the center of the screen.

You can poll it at each loop with canvascv::MsgBox::getUserSelection() or you can use it
in a modal way, in which you block everything else and wait on that line of code for the user selection.

This is what we'll do here, with this code:
~~~~~~~{.cpp}
#include <canvascv/canvas.h>
#include <canvascv/widgets/msgbox.h>

using namespace canvascv;

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

    Canvas c("MsgBox example", image.size());
    c.enableScreenText();

    namedWindow("MsgBox example", WINDOW_AUTOSIZE);

    c.setMouseCallback(); // optional for mouse usage (see also example_selectbox.cpp)

    Mat out;
    int userSelection = 0;
    int cnt = 0;
    do
    {
        c.redrawOn(image, out);  // draw the canvas on the image copy

        imshow("MsgBox example", out);

	// the blocking API handles GUI internally
        userSelection = MsgBox::create(c, string("Notice #") + to_string(++cnt) + " this msg", {
                                           "Ok", "Whatever"
                                       })->getUserSelection(true);
        c.setScreenText(CCV_STR("User pressed button with index '" << userSelection << "'\n\n" <<
                                "(Choose 'Whatever' to exit)"));
    } while (userSelection == 0);

    destroyAllWindows();

    return 0;
}
~~~~~~~
Notes:
* We needed canvascv::Canvas::setMouseCallback since the Canvas will intercept mouse events for the MsgBox now.
* All widgets have a static create methods, which is the only way to create them.
* The canvascv::MsgBox::create will return a shared_ptr<MsgBox> instance, which you don't have to keep since another one is kept by the canvascv::Layout.
* Here we're using getUserSelection(true) immediatly on that shared_ptr, and by passing true we're blocked at that line of code.
* *CCV_STR* lets you write create a string as writing into a stream.
* When executed with a path to an image, this gives you (depends on your image):
@image html tut_msgbox_modal.png
<BR>

@section tmbsec3 A non modal MsgBox

Remeber thah the MsgBox is a "one shot user pressed and widget died" kind of widget. Here we're recreating the MsgBox
each time it dies, but with callbacks.

If you wan to see code also using the MsgBox without callback (polling) then see @ref example_msgbox.cpp.

Here we'll be working with callbacks. The callback is called when the user presses a button.

After the callback is called the MsgBox will be destroyed automatically by the framework.

In our callback we'll immediatly create a new MsgBox with the same callback, just changing the message.

Add these lines to the 'main loop reminder code' above, after creating the namedWindow():
~~~~~~~{.cpp}
using namespace canvascv;
    //...

    c.setMouseCallback(); // optional for mouse usage (see also example_selectbox.cpp)

    Widget::CBUserSelection  cb;
    int cnt = 0;

    // initialize the cb with a C++11 lambda expression.
    // this will be given to the MsgBoxes we'll create.
    cb = [&c, &cb, &cnt](Widget *w, int index)
    {
        MsgBox *pMsgBox = (MsgBox*) w;
        c.setScreenText(CCV_STR( "User pressed option numer '" << index << "'\n" <<
                                 "The button text is '" << pMsgBox->getTextAt(index) << "'\n\n" <<
                                 "(press q to exit)"));
        MsgBox::create(c, CCV_STR("Do you really want to do that? " << "(" << ++cnt << ")"), {
                           "Yes", "No"
                       }, cb);
    };

    // the first MsgBox the user will see is this one.
    // when it ends by a mouse press, the cb will be called to create a new MsgBox.
    MsgBox::create(c, "Do you really want to do that?", {"Yes", "No"}, cb);

    //...
~~~~~~~

Notes:
* All widgets have a static create methods, which is the only way to create them.
* The canvascv::MsgBox::create will return a shared_ptr<MsgBox> instance, which you don't have to keep since another one is kept by the canvascv::Layout.
* This tutorial is using C++11 lambda expressions as callbacks, but anything which has the void(Widget*,int) signature will work.
* We needed canvascv::Canvas::setMouseCallback since the Canvas will intercept mouse events for the MsgBox now.
* *CCV_STR* lets you write create a string as writing into a stream.
* When executed with a path to an image, this gives you (depends on your image):
@image html tut_msgbox.png
<BR>

@section tmbsec4 An external window modal MsgBox
Since a MsgBox usually requires immediate user attention, you have another way to block other GUI while waiting for a user reaction.

This API opens an new independent OpenCV window, occupying just the MsgBox, and waiting for a user response.

This is a blocking API, the code will wait for a user button press.

(You actually saw something like this when we explained the canvascv::Canvas::fatal() in a previous tutorial)


~~~~~~~{.cpp}
#include <canvascv/canvas.h>
#include <canvascv/widgets/msgbox.h>

using namespace canvascv;

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

    Canvas c("MsgBox example", image.size());
    c.enableScreenText();

    namedWindow("MsgBox example", WINDOW_AUTOSIZE);

    c.setMouseCallback(); // optional for mouse usage (see also example_selectbox.cpp)

    Mat out;
    int userSelection = 0;
    do
    {
        c.redrawOn(image, out);  // draw the canvas on the image copy

        imshow("MsgBox example", out);

	// the blocking API handles GUI internally
        userSelection = MsgBox::createModal("Modal MsgBox", "Notice this window", {"Ok", "Whatever"});
        c.setScreenText(CCV_STR("User pressed button with index '" << userSelection << "'\n\n" <<
                                "(Choose 'Whatever' to exit)"));
    } while (userSelection == 0);

    destroyAllWindows();

    return 0;
}
~~~~~~~

Notes:
* Again - this is a blocking API. The line which is using canvascv::MsgBox::createModal waits for a response.
* Currently there is no control of where the OpenCV window will be opened.
* *CCV_STR* lets you write create a string as writing into a stream.
* When executed with a path to an image, this gives you (depends on your image):
@image html tut_msgbox_modal_external.png
<BR>

**That's all for this tutorial**
