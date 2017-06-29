---
layout: page
title: About
permalink: /about/
---

What is it?
----------
* A non intrusive, simple and extendable OpenCV GUI system, based on OpenCV.
* Enables a user interface with interactive GUI without any external GUI toolkit dependencies.
* Get geomertical user selections and landmark configuration with shapes.
* Use simple widgets to interact with the user.

What makes it unique?
--------------------
* Easily extendable - create your own Shapes, Widgets and Themes and share them with others (i.e. on GitHub) if you want.
* The X/Y coordination system is the same as the OpenCV coordination system. This way, if you detect someting at a position, you can start showing
some Text at that exact postion, or if you want the user to mark someting on the image, you get back the usable coordinates from the shape.
* No external dependencies - **your code is compatible to all OpenCV environments**.
* Fully documented with examples and tutorials.

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
* Text
* Button
* RadioButton
* SelectionBox
* MsgBox
* CheckBoxes
* RadioButtons
* Layouts: VerticalLayout, HorizontalLayout, Canvas (actually canvas is not a widget, but it is a layout)
* VFrame (VerticalLayout based Frame)
* HFrame (HorizontalLayout based Frame)
* (FileBrowser - primitive implementation as an example of adding a widget)

### Capabilities:
* Widgets are semi transparent by default.
* All shapes are draggable.
* All shapes are editable.
* Can save/load shapes to/from a file (if the user has set landmarks, we can save them).
* Callbacks for all shapes (create/change/delete).
* Callbacks per shapes (selected/unselected/removed).
* Callbacks per widget events (enter/leave/press/release).
* There is a a theme framework with a predefined theme. User external themes are supported.
* Simple auto lower screen status msg for shapes, widgets and you.
* Upper screen area user messages for you.

### Many examples:
* Example of using shapes.
* Example per widget.
* Example of adding a widget.
* Example of adding themes.
* Example of shapes and widgets together.

### List of things left to do
-------------------------
* Improve current widgets (sub selection lists, icons on buttons, ...)
* Add widgets (Bars, Seperators, Diagrams, Table, ...)
* Add themes
* Improve themeing (support for system icons themes)
* Support more mouse events (right click, middle click, mouse wheel, control+mouse)
* Add a GridLayout
* Improve callbacks on widgets and shapes
* Add Python extensions
* Freetype font support
* Scrollable widgets
* Draggable widgets
* Add a widget designer application
