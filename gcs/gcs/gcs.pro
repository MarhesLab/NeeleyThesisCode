TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp ./QgcsThread/qgcsthread.cpp \
    ../../defs.cpp \
    ../../socket/packet.cpp \
    ../../socket/QuadServer.cpp \
    ../../thread/GcsServerThread.cpp
QMAKE_CXXFLAGS += -std=c++0x

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES +=

FORMS +=

HEADERS += ./QgcsThread/qgcsthread.h \
    ../../defs.h \
    ../../socket/packet.h \
    ../../socket/QuadServer.h \
    ../../thread/GcsServerThread.h
