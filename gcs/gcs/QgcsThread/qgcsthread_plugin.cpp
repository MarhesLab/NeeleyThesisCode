#include "qgcsthread_plugin.h"
#include "qgcsthread.h"

#include <qqml.h>

void QgcsThreadPlugin::registerTypes(const char *uri)
{
    // @uri com.mycompany.qmlcomponents
    qmlRegisterType<QgcsThread>(uri, 1, 0, "QgcsThread");
}


