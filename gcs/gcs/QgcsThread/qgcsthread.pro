TEMPLATE = lib
TARGET = QgcsThread
QT += qml quick
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.mycompany.qmlcomponents

# Input
SOURCES += \
    qgcsthread_plugin.cpp \
    qgcsthread.cpp \
    ../../../thread/GcsServerThread.cpp \
    ../../../defs.cpp \
    ../../../socket/packet.cpp \
    ../../../socket/QuadServer.cpp
QMAKE_CXXFLAGS += -std=c++0x

HEADERS += \
    qgcsthread_plugin.h \
    qgcsthread.h \
    ../../../thread/GcsServerThread.h \
    ../../../defs.h \
    ../../../socket/packet.h \
    ../../../socket/QuadServer.h

DISTFILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

