#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QXmlStreamReader>
#include <QFile>
#include <QCoreApplication>
#include <QDir>


class Server: QTcpServer
{
    Q_OBJECT

public:
    Server();

    QTcpSocket* socket;
    QFile file;
    QXmlStreamReader xmlSR;

    void readNextXML(QXmlStreamReader* xmlSR);
public slots:
    void startServer();
    void incomingConnection(qintptr socket_descriptor);
    void socketReady();
    void socketDisconnect();
};

#endif // SERVER_H
