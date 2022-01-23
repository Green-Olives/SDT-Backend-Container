#include "requesthandler.h"
#include <QtGlobal>
#include <iostream>
//#include <QDebug>
RequestHandler::RequestHandler(QObject *parent)
    : HttpRequestHandler(parent), db(QSqlDatabase::addDatabase("QPSQL")), connected(false)
{
    // если по имени сервиса
//    db.setHostName("http://backend); этот кастрированно выглядит, не понятен порт
//    db.setPort(5432);

//    db.setHostName("127.0.0.1");
//    db.setPort(5432);
//    db.setDatabaseName("mydb");
//    db.setUserName("dan");
//    db.setPassword("trewtrew");

    // если по переменным среды
    QString envGetService = QString::fromLocal8Bit(qgetenv("POSTGRES_HOST"));
    if (envGetService.isEmpty())
        qFatal("envGetService is empty");
    envGetService = envGetService.toUpper();
    envGetService.replace('-', '_');
    QString envGetHost = envGetService + "_SERVICE_HOST";
    QString envGetPort = envGetService + "_SERVICE_PORT";
    QString host = QString::fromLocal8Bit(qgetenv(envGetHost.toStdString().c_str()));
    bool ok;
    int port = QString::fromLocal8Bit(qgetenv(envGetPort.toStdString().c_str())).toInt(&ok);
    if (!ok) {
        qFatal("cannot convert port to int");
    }
    std::cout << "host: " << host.toStdString() << std::endl;
    std::cout << "port: " << port << std::endl;

    QString dbName = QString::fromLocal8Bit(qgetenv("POSTGRES_DB"));
    if (dbName.isEmpty())
        qFatal("database name is empty");
    std::cout << "dbName:" << dbName.toStdString() << std::endl;
    QString userName = QString::fromLocal8Bit(qgetenv("POSTGRES_USER"));
    if (userName.isEmpty())
        qFatal("postgres-user is empty");
    std::cout << "username is: " << userName.toStdString() << std::endl;
    QString pass = QString::fromLocal8Bit(qgetenv("POSTGRES_PASSWORD"));
    if (pass.isEmpty())
        qFatal("postgres-password is empty");
    std::cout << "password is: " << pass.toStdString() << std::endl;
    db.setHostName(host);
    db.setPort(port);
    db.setDatabaseName(dbName);
    db.setUserName(userName);
    db.setPassword(pass);
    if (!db.open()) {
        qCritical() << db.lastError().text();
        qCritical() << db.lastError().driverText();
        qFatal("cant open database");
    }
    //db.exec(); //проверка на наличие базы данных, если её нет, то создается бд с таблицами.
//    if (!db.open()) {
//        qCritical() << db.lastError().text();
//        qFatal("cant open db");
//    }
    QSqlQuery q;
    if (!q.exec("select * from public.tmp")) {
        if ((q.lastError().type() == QSqlError::ErrorType::TransactionError) && !q.exec("create table public.tmp(a integer)"))
            qFatal("cant create table");
    }
    if (!q.exec("insert into public.tmp values(1)"))
        qFatal("cant insert into table");
}

RequestHandler::~RequestHandler()
{
    QSqlQuery q;
    q.exec("drop table public.tmp");
}

void RequestHandler::service(HttpRequest &request, HttpResponse &response)
{
    QSqlQuery q;
//    request.getParameter("username");
    if (!q.exec("select * from public.tmp"))
        qFatal("cant get data from table");
    q.next();
    QString val = q.value(0).toString();
    std::cout << "selected value is: " << val.toStdString() << std::endl;
    response.write("selected " + val.toLatin1(), true);
//    QByteArray username=request.getParameter("username");
//    QByteArray password=request.getParameter("password");

//    qDebug("username=%s",username.constData());
//    qDebug("password=%s",password.constData());
//    response.setHeader("Content-Type", "text/html; charset=ISO-8859-1");
//    response.write("<html><body>");

//    if (isCorrect(username, password)) {
//        response.write("Yes, correct");
//    }
//    else {
//        response.write("<form method='POST' action='/login'>");
//        if (!username.isEmpty()) {
//            response.write("No, that was wrong!<br><br>");
//        }
//        response.write("Please log in:<br>");
//        response.write("Name:  <input type='text' name='username'><br>");
//        response.write("Password: <input type='password' name='password'><br>");
//        response.write("<input type='submit'>");
//        response.write("</form");
//    }

//    response.write("</body></html>",true);
}

void RequestHandler::connection()
{
//    connected = true;
//    qDebug() << "connected";
}

//bool RequestHandler::isCorrect(QByteArray &username, QByteArray &password)
//{
////    QTcpSocket *s = new QTcpSocket(this);
////    connect(s, SIGNAL(connected()), this, SLOT(connection()));
////    s->connectToHost("127.0.0.1", 10000);
////    if (s->waitForConnected(10000))
////        qDebug() << "connection ok" << username << password;
//}

