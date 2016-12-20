QT += core
QT -= gui qt

CONFIG += c++11

TARGET = My-3D-Scanner
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

LIBS += -lopencv_highgui -lopencv_core -lopencv_core -lopencv_features2d \
        -lopencv_flann -lopencv_imgcodecs -lopencv_imgproc -lopencv_calib3d \
        -lopencv_photo -lopencv_shape -lopencv_superres -lopencv_stitching

SOURCES += main.cpp \
    PhaseShiftProcess.cpp

HEADERS += \
    PhaseShiftProcess.h
