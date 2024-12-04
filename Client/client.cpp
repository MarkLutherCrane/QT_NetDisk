#include "client.h"
#include "ui_client.h"
#include "index.h"
#include <QFile>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>
#include "protocol.h"
Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    loadConfig();
    QObject::connect(&m_tcpSocket, &QTcpSocket::connected, this, &Client::showConnected);
    connect(&m_tcpSocket, &QTcpSocket::readyRead, this, &Client::recvMsg);
    m_tcpSocket.connectToHost(QHostAddress(m_strIP), m_usPort);

    m_prh = new ResHandler();
}

QTcpSocket &Client::getSocket()
{
    return m_tcpSocket;
}

Client &Client::getInstance()
{
    static Client instance;
    return instance;
}

Client::~Client()
{
    delete ui;
}

void Client::loadConfig()
{
    //
    QFile file(":/client.config");
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

void Client::sendMsg(PDU *pdu)
{
    int ret = m_tcpSocket.write((char*)pdu, pdu->uiPDULen);
    qDebug() << "ret = " << ret;
    qDebug() << "send uiMsgType: " << pdu->uiMsgType
             << "uiPDULen: " << pdu->uiPDULen
             << "uiMsgLen" << pdu->uiMsgLen
             << "caData: " << pdu->caData
            << "caData + 32: " << pdu->caData + 32
             << "caMsg: " << pdu->caMsg;
    free(pdu);
    pdu = nullptr;
}

PDU *Client::readMsg()
{
    qDebug() << "recvMsg 接收消息长度: " << m_tcpSocket.bytesAvailable();
    uint uiPDULen = 0;
    m_tcpSocket.read((char*)&uiPDULen, sizeof(uiPDULen));  // 读协议总长度
    uint uiMsgLen = uiPDULen - sizeof(PDU);
    PDU* pdu = mkPDU(0, uiMsgLen);
    m_tcpSocket.read((char*)pdu+sizeof(uiPDULen), uiPDULen - sizeof(uiPDULen));  // 读取除了协议总长度以外剩余的内容
    pdu->caMsg[pdu->uiMsgLen] = '\0';

    return pdu;

}

void Client::handleMsg(PDU *pdu)
{
    qDebug() << "recvMsg uiMsgType: " << pdu->uiMsgType
             << "uiPDULen: " << pdu->uiPDULen
             << "uiMsgLen" << pdu->uiMsgLen
             << "caData: " << pdu->caData
             << "caData + 32: " << pdu->caData + 32
             << "caMsg: " << pdu->caMsg;
    m_prh->pdu = pdu;
    switch(pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_REGIST_RESPOND:
    {
        m_prh->handleRegist();
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_RESPOND:
    {
        m_prh->handleLogin();
        break;
    }
    case ENUM_MSG_TYPE_FIND_USER_RESPOND:
    {
        m_prh->handleFindUser();
        break;
    }
    case ENUM_MSG_TYPE_ONLINE_USER_RESPOND:
    {
        m_prh->handleOnlineUser();
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
    {
        m_prh->handleAddFriendResend();
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND:
    {
        m_prh->handleAddFriend();
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPOND:
    {
        m_prh->handleAddFriendAgree();
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND:
    {
        m_prh->handleFlushFriend();
        break;
    }
    case ENUM_MSG_TYPE_CHAT_REQUEST:
    {
        m_prh->handleChatResend();
        break;

    }
    case ENUM_MSG_TYPE_MKDIR_RESPOND:
    {
        m_prh->handleChatMkdir();
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FILE_RESPOND:
    {
        m_prh->handleFlushFile();
        break;
    }
    case ENUM_MSG_TYPE_DEL_DIR_RESPOND:
    {
        m_prh->handleDelDir();
        break;

    }
    case ENUM_MSG_TYPE_MV_FILE_RESPOND:
    {
        m_prh->handleMoveFile();
        break;
    }
    case ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND:
    {
        m_prh->handleUploadFileInit();
        break;
    }
    case ENUM_MSG_TYPE_UPLOAD_FILE_DATA_RESPOND:
    {
        m_prh->handleUploadData();
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_REQUEST:
    {
        m_prh->handleShareFileResend();
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPOND:
    {
        m_prh->handleShareFileAgree();
        break;
    }
    default:
        break;
    }
}

void Client::showConnected()
{
    qDebug() << "Connect Server success";
}

void Client::recvMsg()
{
    qDebug() << "recvMsg 接收消息长度: " << m_tcpSocket.bytesAvailable();
    QByteArray data = m_tcpSocket.readAll();
    buffer.append(data);  // 半包志存下来，拼成一个完成包在进入while处理
    // 判断buffer中的数据够不够一个PDU结构体
    while (buffer.size() >= int(sizeof(PDU)))
    {
        PDU* pdu = (PDU*)buffer.data();
        // 判断buffer中的数据够不够一个完整包
        if (buffer.size() < int(pdu->uiPDULen))
        {
            break;
        }
        // 处理一个完整包的数据
        handleMsg(pdu);
        // 移除处理完整包
        buffer.remove(0, pdu->uiPDULen);
    }
}

void Client::uploadError(const QString &error)
{
    QMessageBox::information(this, "提示", error);

}


void Client::on_regist_PB_clicked()
{
    QString strName = ui->name_LE->text();
    QString strPwd = ui->pwd_LE->text();
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_REGIST_REQUEST, 0);
    memcpy(pdu->caData, strName.toStdString().c_str(), 32);
    memcpy(pdu->caData + 32, strPwd.toStdString().c_str(), 32);

    sendMsg(pdu);
}

void Client::on_login_PB_clicked()
{
    QString strName = ui->name_LE->text();
    QString strPwd = ui->pwd_LE->text();
    m_strLoginName = strName;
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_LOGIN_REQUEST, 0);
    memcpy(pdu->caData, strName.toStdString().c_str(), 32);
    memcpy(pdu->caData + 32, strPwd.toStdString().c_str(), 32);

    sendMsg(pdu);
}

QString Client::getRootDir()
{
    return m_strRootDir;
}
