TEMPLATE = app
CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += -std c++11

INCLUDEPATH += /usr/lib/boost_1_72_0

LIBS += -L/usr/lib/boost_1_72_0 -lpthread

SOURCES += main.cpp \
    network_utils.cpp \
    LocalClient.cpp

HEADERS += \
    network_utils.h \
    LocalClient.h

