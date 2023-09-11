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
#include <QSqlRecord>
#include <QSqlField>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>
#include <QCryptographicHash>


class Server: QTcpServer
{
    Q_OBJECT

public:
    Server();

    QTcpSocket* socket;
    QFile file;
    QXmlStreamReader xmlSR;
    QSqlDatabase db;
    QTimer* timer;
    QMultiMap<QString, QByteArray> fileHashMap;

    void removeDBTables(QSqlQuery* query);
    void createDBTables(QSqlQuery* query);
    void xmlToDB();
    void xmlToDB(QXmlStreamReader* xmlSR, QSqlQuery *query, QString parent_id);
    QJsonDocument dbToJson();
    QJsonArray dbToJson(QSqlQuery* query);
public slots:
    void startServer();
    void incomingConnection(qintptr socket_descriptor);
    void socketReady();
    void socketDisconnect();
    void checkXML();
};

#endif // SERVER_H
