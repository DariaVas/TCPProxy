TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/lib/boost_1_72_0 ../utils

LIBS += -L/usr/lib/boost_1_72_0 -lpthread


SOURCES += main.cpp \
    ../utils/network_utils.cpp \
    LocalServer.cpp

HEADERS += \
    ../utils/network_utils.h \
    LocalServer.h

