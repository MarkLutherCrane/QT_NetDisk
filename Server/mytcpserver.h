#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QThreadPool>
#include "mytcpsocket.h"
class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    QThreadPool m_threadPool;

    static MyTcpServer& getInstance();
    void incomingConnection(qintptr handle);
    void resend(char *caTarName, PDU* pdu);
public slots:
    void removeOfflineTcpSocket_slot(MyTcpSocket *myTcpSocket);
private:
    MyTcpServer();
    MyTcpServer(const MyTcpServer&) = delete;
    MyTcpServer& operator=(const MyTcpServer&) = delete;
    QList<MyTcpSocket*> m_tcpSocketList;
};

#endif // MYTCPSERVER_H
