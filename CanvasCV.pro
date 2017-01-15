#TEMPLATE = app
TEMPLATE = lib
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11

#SOURCES += main.cpp \
SOURCES += \
    canvas.cpp \
    line.cpp \
    shapefactory.cpp \
    compoundshape.cpp \
    colors.cpp \
    handle.cpp \
    arrow.cpp \
    linecrossing.cpp \
    shape.cpp \
    textbox.cpp \
    trapezoid.cpp \
    quadrilateral.cpp

# pkgconfig opencv --libs
LIBS += -L/usr/local/lib -L/usr/local/share/OpenCV/3rdparty/lib -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv -lopencv_dnn -lopencv_dpm -lopencv_fuzzy -lopencv_line_descriptor -lopencv_optflow -lopencv_plot -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_rgbd -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_face -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lippicv -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core

INCLUDEPATH += /usr/local/include

HEADERS += \
    canvas.h \
    line.h \
    shape.h \
    shapefactory.h \
    compoundshape.h \
    handle.h \
    colors.h \
    arrow.h \
    linecrossing.h \
    textbox.h \
    trapezoid.h \
    quadrilateral.h

OTHER_FILES += \
    CMakeLists.txt
