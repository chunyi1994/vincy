TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS +=  -Wno-unused-parameter
QMAKE_CXXFLAGS +=  -Wno-unused-function

LIBS += -lpthread
LIBS += -lboost_system
LIBS += -lboost_filesystem
SOURCES += main.cpp \
    tcp_connection.cpp \
    http_message.cpp \
    http_server.cpp \
    http_handler.cpp \
    simple_template.cpp \
    http_response.cpp \
    http_request.cpp \
    session_manager.cpp

HEADERS += \
    tcp_connection.h \
    log.h \
    http_message.h \
    utils.h \
    http_server.h \
    http_handler.h \
    http_utils.h \
    simple_template.h \
    cookie.h \
    http_response.h \
    http_request.h \
    session_manager.h \
    singleton.h

