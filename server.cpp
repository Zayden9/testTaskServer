#include "server.h"

Server::Server() {}

void Server::startServer()
{
    if (this->listen(QHostAddress("127.0.0.1"), 5555))
    {
        qDebug() << "Server started";
        file.setFileName(QCoreApplication::applicationDirPath() + "/1.1.1.1.xml");
        if (file.open(QIODevice::ReadWrite))
        {
            xmlSR.addData(file.readAll());

            while (!xmlSR.atEnd())
            {
                readNextXML(&xmlSR);
                qDebug() << xmlSR.name();
                for(int i = 0; i < xmlSR.attributes().count(); i++)
                {
                    qDebug() << xmlSR.attributes().at(i).name();
                }
            }
        }
    }
    else
    {
        qDebug() << "Server not started";
    }

}

void Server::incomingConnection(qintptr socket_descriptor)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socket_descriptor);
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnect()));

    qDebug() << socket_descriptor << " is connected";

    socket->write("you are connected");

    qDebug() << "write to client: you are connected";
}

void Server::socketReady()
{

}

void Server::socketDisconnect()
{
    qDebug() << "smb is disconnected";
    socket->deleteLater();
}

void Server::readNextXML(QXmlStreamReader* xmlSR)
{
    while (!xmlSR->readNextStartElement() && !xmlSR->atEnd());
}
