#include "mytcpsocket.h"
#include "protocol.h"
#include "operatedb.h"
MyTcpSocket::MyTcpSocket()
{

    m_prh = new ReqHanler();
}

MyTcpSocket::~MyTcpSocket()
{
    delete m_prh;
}

void MyTcpSocket::sendMsg(PDU *pdu)
{
    if (!pdu)
    {
        qDebug() << "sendMsg errno";
        return;
    }
    write((char*)pdu, pdu->uiPDULen);
    qDebug() << "send uiMsgType: " << pdu->uiMsgType
             << "uiPDULen: " << pdu->uiPDULen
             << "uiMsgLen" << pdu->uiMsgLen
            << "caData: " << pdu->caData
            << "caMsg: " << pdu->caMsg;

    free(pdu);
    pdu = nullptr;
}

PDU* MyTcpSocket::handleMsg(PDU *pdu)
{
    qDebug() << "recvMsg uiMsgType: " << pdu->uiMsgType
             << "uiPDULen: " << pdu->uiPDULen
             << "uiMsgLen" << pdu->uiMsgLen
             << "caData: " << pdu->caData
             << "caMsg: " << pdu->caMsg;
    m_prh->pdu = pdu;
    switch(pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_REGIST_REQUEST:
    {
        return m_prh->handleRegist();
    }
    case ENUM_MSG_TYPE_LOGIN_REQUEST:
    {
        return m_prh->handleLogin(m_strLoginName);
    }
    case ENUM_MSG_TYPE_FIND_USER_REQUEST:
    {
        return m_prh->handleFindUser();
    }
    case ENUM_MSG_TYPE_ONLINE_USER_REQUEST:
    {
        return m_prh->handleOnlineUser();
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
    {
        return m_prh->handleAddFriend();
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST:
    {
        return  m_prh->handleAddFriendAgree();
    }
    case ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST:
    {
        return  m_prh->handleFlushFriend();
    }
    case ENUM_MSG_TYPE_CHAT_REQUEST:
    {
        return  m_prh->handleChat();
    }
    case ENUM_MSG_TYPE_MKDIR_REQUEST:
    {
        return  m_prh->handleMkdir();
    }
    case ENUM_MSG_TYPE_FLUSH_FILE_REQUEST:
    {
        return  m_prh->handleFlushFile();
    }
    case ENUM_MSG_TYPE_DEL_DIR_REQUEST:
    {
        return  m_prh->handleDelDir();
    }
    case ENUM_MSG_TYPE_MV_FILE_REQUEST:
    {
        return  m_prh->handleMoveFile();
    }
    case ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST:
    {
        return  m_prh->handleUpLoadInit();
    }
    case ENUM_MSG_TYPE_UPLOAD_FILE_DATA_REQUEST:
    {
        return  m_prh->handleLoadFileData();
    }
    case ENUM_MSG_TYPE_SHARE_FILE_REQUEST:
    {
        return  m_prh->hanleShareFile();
    }
    case ENUM_MSG_TYPE_SHARE_FILE_AGREE_REQUEST:
    {
        return  m_prh->hanleShareFileAgree();
    }
    default:
        break;
    }
    return nullptr;
}


PDU *MyTcpSocket::readMsg()
{
    qDebug() << "recvMsg 接收消息长度: " << this->bytesAvailable();
    uint uiPDULen = 0;
    this->read((char*)&uiPDULen, sizeof(uiPDULen));  // 读协议总长度
    uint uiMsgLen = uiPDULen - sizeof(PDU);
    PDU* pdu = mkPDU(0, uiMsgLen);
    this->read((char*)pdu+sizeof(uiPDULen), uiPDULen - sizeof(uiPDULen));  // 读取除了协议总长度以外剩余的内容
    pdu->caMsg[pdu->uiMsgLen] = '\0';
    qDebug() << "recvMsg uiMsgType: " << pdu->uiMsgType
             << "uiPDULen: " << pdu->uiPDULen
             << "uiMsgLen" << pdu->uiMsgLen
             << "caData: " << pdu->caData
             << "caMsg: " << pdu->caMsg;
    return pdu;
}

void MyTcpSocket::clientOffline()
{
    bool ret = OperateDB::getInstance().handleOffine(m_strLoginName.toStdString().c_str());
    qDebug() << "handleOffine: ret = " << ret;
    emit removeOfflineTcpSocket_signal(this);
}

void MyTcpSocket::recvMsg()
{
    qDebug() << "recvMsg 接收消息长度: " << this->bytesAvailable();
    QByteArray data = this->readAll();
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

        PDU *respdu = handleMsg(pdu);
        sendMsg(respdu);
        buffer.remove(0, pdu->uiPDULen);
    }

}
