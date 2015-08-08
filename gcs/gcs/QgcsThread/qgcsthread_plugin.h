#ifndef QGCSTHREAD_PLUGIN_H
#define QGCSTHREAD_PLUGIN_H

#include <QQmlExtensionPlugin>

class QgcsThreadPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // QGCSTHREAD_PLUGIN_H

