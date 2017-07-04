Auto widget layouts {#tutlayouts}
===================
In the previous tutorials we've positioned the Widgets independetly.

In many cases you want a set of widgets to be located together in a certain order.

For this you need AutoLayout derived classes:
* canvascv::HorizontalLayout
* canvascv::VerticalLayout
* canvascv::HFrame
* canvascv::VFrame

In this tutorial we'll return to some previous tutorial code and see how
to do it in a layout. Then we'll write a simple form which gets inputs
from the user.

[TOC]

# Center text on the screen
In this tutorial @ref screentext_s2_3 we created a Text widget and
manually tried to center it on the screen.

To center it vertically and horizontally, you would use 2 layout
managers - canvascv::VerticalLayout and canvascv::HorizontalLayout.


