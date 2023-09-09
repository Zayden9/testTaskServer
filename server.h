#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>


class Server: QTcpServer
{
    Q_OBJECT

public:
    Server();

    QTcpSocket* socket;
public slots:
    void startServer();
    void incomingConnection(qintptr socket_descriptor);
    void socketReady();
    void socketDisconnect();
};

#endif // SERVER_H
