#include <unistd.h>     // sleep and usleep
#include <iostream>
#include <QString>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QtQml>

#include "QgcsThread/qgcsthread.h"

using namespace std;

int main(int argc, char *argv[])
{
    // Application Initialization
    QApplication app(argc, argv);

    // Registers the custom QtQuick object for doing the GCS's IP socket threading
    qmlRegisterType<QgcsThread>("com.wilneeleyisabadass.qmlcomponents", 1, 0, "GcsThread");

    QQmlApplicationEngine engine;
    //QQmlComponent *component = new QQmlComponent(&engine);

    // signal and slot connections
    //QQuickItem::connect(&engine, SIGNAL(quit()), QCoreApplication::instance, SLOT(quit()));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
