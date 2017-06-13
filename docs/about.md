---
layout: page
title: About
permalink: /about/
---

What is it?
----------
* A non intrusive simple GUI system for OpenCV, based on OpenCV.
* Enables a user interface with interactive GUI without any external GUI toolkit dependencies.
* Get geomertical user selections with shapes.
* Use simple widgets to interact with the user.

What makes it unique?
--------------------
* The X/Y coordination system is the same as the OpenCV coordination system. This way, if you detect someting at a position, you can start showing
some FloatingText at that exact postion, Or if you want the user to mark someting on the image, you get back the exact coordinates.
* No external dependencies - **your code is compatible to all OpenCV environments**.

## Supported Shapes, Widgets and capabilities

### Supported interactive shapes:
* Line
* Arrow
* TextBox
* LineCrossing (directional by user, so you can tell if something crossed in a specific direction)
* Polygon
* Rectangle
* Ellipse
* ShapesConnector
* LabeledShapesConnector

### Supported widgets:
* FloatingText
* Button
* RadioButton
* SelectionBox
* PopUp
* VerticalLayout
* HorizontalLayout
* VFrame (VerticalLayout based Frame)
* HFrame (HorizontalLayout based Frame)

### Capabilities:
* Widgets are semi transparent by default.
* All shapes are draggable.
* All shapes are editable.
* Callbacks both for all shapes (create/change/delete).
* Callbacks per shapes (selected/unselected/removed).
* Callbacks for widget events (enter/leave/press/release).
* There is a a theme framework with a predefined theme. User external themes are supported.
* Simple auto lower screen status msg for shapes, widgets and you.
* Upper screen area user messages for you.


