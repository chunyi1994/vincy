TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11
LIBS += -lpthread
LIBS += -lboost_system
LIBS += -lboost_filesystem
SOURCES += main.cpp \
    tcp_connection.cpp \
    http_message.cpp \
    http_server.cpp \
    http_handler.cpp \
    simple_template.cpp

HEADERS += \
    tcp_connection.h \
    log.h \
    http_message.h \
    utils.h \
    http_server.h \
    http_handler.h \
    http_utils.h \
    simple_template.h

