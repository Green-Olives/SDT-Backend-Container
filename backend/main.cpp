#include <QCoreApplication>
#include "httplistener.h"
#include "requesthandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QSettings* listenerSettings = new QSettings(QString("../%1/etc/webapp1.ini").arg(app.applicationName()),QSettings::IniFormat/*,&app*/);

    listenerSettings->beginGroup("listener");
    new HttpListener(listenerSettings, new RequestHandler(&app), &app);

    return app.exec();
}
