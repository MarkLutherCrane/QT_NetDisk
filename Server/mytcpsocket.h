#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include <qtcpsocket.h>
#include "protocol.h"
#include "reqhanler.h"
class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    ~MyTcpSocket();
    void sendMsg(PDU* pdu);
    PDU* handleMsg(PDU* pdu);
    PDU* readMsg();

    QString m_strLoginName;
    ReqHanler* m_prh;
    QByteArray buffer;

public slots:
    void recvMsg();  // 接收并处理消息的函数
    void clientOffline();  // 离线处理
signals:
    void removeOfflineTcpSocket_signal(MyTcpSocket *myTcpSocket);
};

#endif // MYTCPSOCKET_H
