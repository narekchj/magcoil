QT += 3dcore 3drender 3dinput 3dextras
QT += widgets
QT += core

QMAKE_CXXFLAGS += -std=c++2a 

SOURCES += main.cpp \
    scenemodifier.cpp \
    curve.cpp \
    data_helper.cpp \
    calc_thread.cpp \


HEADERS += \
    scenemodifier.h \
    calc_thread.h
