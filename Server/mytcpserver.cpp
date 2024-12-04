#include "clienttask.h"
#include "mytcpserver.h"

MyTcpServer::MyTcpServer()
{
    m_threadPool.setMaxThreadCount(16);
}


MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr handle)
{
    qDebug() << "新的客户端连接成功";
    // 每一个新的客户端连接，都将其放入tcpSocket连接列表中
    MyTcpSocket *p = new MyTcpSocket;
    p->setSocketDescriptor(handle);

    QTcpSocket *socket = qobject_cast<QTcpSocket*>(p);
    if (socket) {
        QString ip = socket->peerAddress().toString();
        quint16 port = socket->peerPort();
        qDebug() << "客户端 IP：" << ip << "，端口：" << port;
    }
    m_tcpSocketList.append(p);

    connect(p, &MyTcpSocket::removeOfflineTcpSocket_signal, this, &MyTcpServer::removeOfflineTcpSocket_slot);

    ClientTask *task = new ClientTask(p);
    m_threadPool.start(task);
}

void MyTcpServer::resend(char *caTarName, PDU *pdu)
{
    if (caTarName == nullptr || pdu == nullptr)
    {
        return;
    }
    for (int i = 0; i < m_tcpSocketList.size(); i++)
    {
        if (caTarName == m_tcpSocketList.at(i)->m_strLoginName)
        {
            m_tcpSocketList.at(i)->write((char*)pdu, pdu->uiPDULen);
            qDebug() << "send uiMsgType: " << pdu->uiMsgType
                     << "uiPDULen: " << pdu->uiPDULen
                     << "uiMsgLen" << pdu->uiMsgLen
                     << "caData: " << pdu->caData
                     << "caMsg: " << pdu->caMsg;
            break;
        }
    }
}

void MyTcpServer::removeOfflineTcpSocket_slot(MyTcpSocket *myTcpSocket)
{
    m_tcpSocketList.removeOne(myTcpSocket);
    myTcpSocket->deleteLater(); // 延迟删除
//    delete myTcpSocket;
    myTcpSocket = nullptr;

    // 测试是否移除成功
    qDebug() << m_tcpSocketList.size();
    for (int i = 0; i < m_tcpSocketList.size(); i++) {
        qDebug() << m_tcpSocketList.at(i)->m_strLoginName;
    }
}
