QT += 3dcore 3drender 3dinput 3dextras
QT += widgets
QT += core

QMAKE_CXXFLAGS += -std=c++2a -O0 -g

SOURCES += main.cpp \
    scenemodifier.cpp \
    magsusp3d.cpp \
    curve.cpp \
    data_helper.cpp \
    calc_thread.cpp \


HEADERS += \
    scenemodifier.h \
    magsusp3d.h \
    calc_thread.h \
    gen_optim.hpp
