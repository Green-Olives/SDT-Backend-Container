#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H
#include "httprequesthandler.h"
#include <QtSql>

using namespace stefanfrings;

class RequestHandler : public HttpRequestHandler
{
    Q_OBJECT
public:
    RequestHandler(QObject *parent = nullptr);
    ~RequestHandler();
    void service(HttpRequest &request, HttpResponse &response);
public slots:
    void connection();
private:
//    bool isCorrect(QByteArray &username, QByteArray &password);
    QSqlDatabase db;
    bool connected;
//    QVector<QTcpSocket*> clients;
};

#endif // REQUESTHANDLER_H
