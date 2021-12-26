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
    QString hostName = QString::fromLocal8Bit(qgetenv("database-host"));
    if (hostName.isEmpty())
        qFatal("host name is empty");
    std::cout << "host name:" << hostName.toStdString() << std::endl;
    QString dbName = QString::fromLocal8Bit(qgetenv("database-name"));
    if (dbName.isEmpty())
        qFatal("database name is empty");
    std::cout << "dbName:" << dbName.toStdString() << std::endl;
    QString userName = QString::fromLocal8Bit(qgetenv("postgres-user"));
    if (userName.isEmpty())
        qFatal("postgres-user is empty");
    std::cout << "username is: " << userName.toStdString() << std::endl;
    QString pass = QString::fromLocal8Bit(qgetenv("postgres-password"));
    if (pass.isEmpty())
        qFatal("postgres-password is empty");
    std::cout << "password is: " << pass.toStdString() << std::endl;
    db.setHostName(hostName);         //это если хост хранится в среде
    db.setPort(5432);                 //а порт стандартный
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

