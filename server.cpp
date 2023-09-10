#include "server.h"

Server::Server() {}

void Server::startServer()
{
    if (this->listen(QHostAddress("127.0.0.1"), 5555))
    {
        qDebug() << "Server started";/*
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
                qDebug() << xmlSR.readNextStartElement();
                qDebug() << xmlSR.readNextStartElement();
                qDebug() << xmlSR.name();
                qDebug() << xmlSR.readNextStartElement();
            }
        }*/
        xmlToDB();
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

void Server::removeDBTables(QSqlQuery* query)
{
    query->exec("DROP TABLE IF EXISTS block;");
    query->exec("DROP TABLE IF EXISTS board;");
    query->exec("DROP TABLE IF EXISTS port;");
}

void Server::createDBTables(QSqlQuery* query)
{
    query->exec("CREATE TABLE 'block' ("
                "'id'	INTEGER NOT NULL,"
                "'Name'	TEXT,"
                "'IP'	TEXT,"
                "'BoardCount'	INTEGER,"
                "'MtR'	INTEGER,"
                "'MtC'	INTEGER,"
                "'Description'	TEXT,"
                "'Label'	TEXT,"
                "PRIMARY KEY('id')"
                ");");
    query->exec("CREATE TABLE 'board' ("
                "'id'	INTEGER NOT NULL,"
                "'parent id'	INTEGER,"
                "'Num'	INTEGER,"
                "'Name'	TEXT,"
                "'PortCount'	INTEGER,"
                "'IntLinks'	TEXT,"
                "'Algoritms'	TEXT,"
                "PRIMARY KEY('id')"
                ");");
    query->exec("CREATE TABLE 'port' ("
                "'id'	INTEGER NOT NULL,"
                "'parent id'	INTEGER,"
                "'Num'	INTEGER,"
                "'Media'	INTEGER,"
                "'Signal'	INTEGER,"
                "PRIMARY KEY('id')"
                ");");
}

void Server::xmlToDB()
{
    QDir dir(QCoreApplication::applicationDirPath());
    QFileInfoList fileIL = dir.entryInfoList((QStringList)"*.xml", QDir::Files);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database.db");
    if (db.open())
    {
        qDebug() << "db open";
        QSqlQuery query(db);
        removeDBTables(&query);
        createDBTables(&query);

        for (int i = 0; i < fileIL.count(); i++)
        {
            file.setFileName(QCoreApplication::applicationDirPath() + "/" + fileIL.at(i).fileName());
            if (file.open(QIODevice::ReadOnly))
            {
                xmlSR.clear();
                xmlSR.addData(file.readAll());
                file.close();
                xmlSR.readNextStartElement();
                xmlSR.readNextStartElement();
                xmlToDB(&xmlSR, &query, "");
            }
        }
        db.close();
        qDebug() << "db close";
    }
    else
    {
        qDebug() << "db not open";
    }
}

void Server::xmlToDB(QXmlStreamReader* xmlSR, QSqlQuery* query, QString parent_id)
{
    qDebug() << "here we are";
    QString s = "INSERT INTO " + xmlSR->name() + " ('";
    for (int i = 0; i < xmlSR->attributes().count(); i++)
    {
        s += xmlSR->attributes().at(i).name();
        if (i < xmlSR->attributes().count()-1) s += "', '";
    }
    if (parent_id != "") s += "', 'parent id";
    s += "') VALUES ('";
    for (int i = 0; i < xmlSR->attributes().count(); i++)
    {
        s += xmlSR->attributes().at(i).value();
        if (i < xmlSR->attributes().count()-1) s += "', '";
    }
    if (parent_id != "") s += ("', '" + parent_id);
    s += "');";
    query->exec(s);
    parent_id = xmlSR->attributes().at(0).value().toString();
    while (xmlSR->readNextStartElement())
    {
        xmlToDB(xmlSR, query, parent_id);
    }
}
