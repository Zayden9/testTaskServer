#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QXmlStreamReader>
#include <QFile>
#include <QCoreApplication>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>


class Server: QTcpServer
{
    Q_OBJECT

public:
    Server();

    QTcpSocket* socket;
    QFile file;
    QXmlStreamReader xmlSR;

    void removeDBTables(QSqlQuery* query);
    void createDBTables(QSqlQuery* query);
    void xmlToDB();
    void xmlToDB(QXmlStreamReader* xmlSR, QSqlQuery *query, QString parent_id);
public slots:
    void startServer();
    void incomingConnection(qintptr socket_descriptor);
    void socketReady();
    void socketDisconnect();
};

#endif // SERVER_H
