QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Hough.cpp \
    src/connexe.cpp \
    src/fileselector.cpp \
    src/image.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/matrice.cpp \
    src/traitement.cpp

HEADERS += \
    Hough.h \
    header/connexe.h \
    header/fileselector.h \
    header/image.h \
    header/mainwindow.h \
    header/matrice.h

FORMS += \
    mainwindow.ui

QMAKE_CXXFLAGS += -fopenmp

unix:INCLUDEPATH += -I$$PWD/include/opencv2
unix:LIBS += -L$$PWD/lib -lopencv_calib3d -lopencv_core -lopencv_dnn -lopencv_features2d -lopencv_flann -lopencv_gapi -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_video -lopencv_videoio -lGLEW -lGLU -lGL -fopenmp
win32:INCLUDEPATH += -I$$PWD\include\opencv2
win32:LIBS += -L$$PWD\lib -lopencv_calib3d -lopencv_core -lopencv_dnn -lopencv_features2d -lopencv_flann -lopencv_gapi -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_video -lopencv_videoio
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
