#include "operatedb.h"
#include "server.h"
#include "ui_server.h"

#include <qfile.h>
#include <QDebug>
#include "mytcpserver.h"
Server::Server(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);
    loadConfig();
    MyTcpServer::getInstance().listen(QHostAddress(m_strIP), m_usPort);
    qDebug() << "server start ";
}


void Server::loadConfig()
{
    //
    QFile file(":/server.config");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray baData = file.readAll();
        QString strData = QString(baData);
        QStringList strList = strData.split("\r\n");
        m_strIP = strList.at(0);
        m_usPort = strList.at(1).toUShort();
        m_strRootDir = strList.at(2);
        qDebug() << "loadConfig "<< "m_strIP: " << m_strIP
                 << " m_usPort: " << m_usPort
                 << " m_strRootDir: " << m_strRootDir;

        file.close();
    } else {
        qDebug() << "load Config error";
    }
}

Server &Server::getInstance()
{
    static Server instance;
    return instance;
}

Server::~Server()
{
    bool ret = OperateDB::getInstance().handleAllOffine();
    qDebug() << "handleAllOffine: " << ret;
    delete ui;
}

QString Server::getRootDir()
{
    return m_strRootDir;
}

