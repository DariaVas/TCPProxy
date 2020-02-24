TEMPLATE = app
CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += -std c++11

INCLUDEPATH += /usr/lib/boost_1_72_0 ../utils

LIBS += -L/usr/lib/boost_1_72_0 -lpthread

SOURCES += main.cpp \
    LocalClient.cpp \
    network_utils.cpp \
    ../utils/network_utils.cpp

HEADERS += \
    LocalClient.h \
    network_utils.h \
    ../utils/network_utils.h

