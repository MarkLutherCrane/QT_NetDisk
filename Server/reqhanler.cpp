#include "mytcpserver.h"
#include "operatedb.h"
#include "reqhanler.h"
#include "server.h"
#include <QDebug>
#include <QDir>
ReqHanler::ReqHanler(QObject *parent) : QObject(parent)
{

}

PDU *ReqHanler::handleRegist()
{
    char caName[32] = "\0";
    char caPwd[32] = "\0";
    memcpy(caName, pdu->caData, 32);
    memcpy(caPwd, pdu->caData + 32, 32);

    qDebug() << "REGIST caName: " << caName
             << "caPwd: " << caPwd;

    bool ret = OperateDB::getInstance().handleResgist(caName, caPwd);
    qDebug() << "handleResgist: ret = " << ret;
    if (ret)
    {
        QDir dir;
        bool isMkdirSuccess = dir.mkdir(QString("%1/%2").arg(Server::getInstance().getRootDir()).arg(caName));
        qDebug() << "isMkdirSuccess: ret = " << isMkdirSuccess;
    }

    pdu = mkPDU(ENUM_MSG_TYPE_REGIST_RESPOND, 0);
    memcpy(pdu->caData, &ret, sizeof(ret));
    return pdu;
}

PDU *ReqHanler::handleLogin(QString &loginName)
{
    char caName[32] = "\0";
    char caPwd[32] = "\0";
    memcpy(caName, pdu->caData, 32);
    memcpy(caPwd, pdu->caData + 32, 32);

    qDebug() << "LOGIN caName: " << caName
             << "caPwd: " << caPwd;
    loginName = caName;
    bool ret = OperateDB::getInstance().handleLogin(caName, caPwd);
    qDebug() << "handleResgist: ret = " << ret;

    PDU *pdu = mkPDU(ENUM_MSG_TYPE_LOGIN_RESPOND, 0);
    memcpy(pdu->caData, &ret, sizeof(ret));
    return pdu;
}

PDU *ReqHanler::handleFindUser()
{
    char caName[32] = "\0";
    memcpy(caName, pdu->caData, 32);

    qDebug() << "FIND UserName: " << caName;
    int ret = OperateDB::getInstance().handleFindUser(caName);
    qDebug() << "handleFindUser: ret = " << ret;

    PDU* pdu = mkPDU(ENUM_MSG_TYPE_FIND_USER_RESPOND, 0);

    memcpy(pdu->caData, caName, 32);
    memcpy(pdu->caData + 32, &ret, sizeof(ret));
    return pdu;
}

PDU *ReqHanler::handleOnlineUser()
{
    qDebug() << "FIND Online UserName: ";
    QStringList res = OperateDB::getInstance().handleOnlineUser();
    qDebug() << "handleOnlineUser: res.size() = " << res.size();

    uint uiMsgLen =res.size() * 32;
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_ONLINE_USER_RESPOND, uiMsgLen);
    for (int i = 0; i < res.size(); i++)
    {
        memcpy(pdu->caMsg + i * 32, res.at(i).toStdString().c_str(), 32);
    }
    return pdu;
}

PDU *ReqHanler::handleAddFriend()
{
    char caCurName[32] = "\0";
    char caTarName[32] = "\0";
    memcpy(caCurName, pdu->caData, 32);
    memcpy(caTarName, pdu->caData + 32, 32);

    qDebug() << "handleAddFriend caName: " << caCurName
             << "caTarName: " << caTarName;
    int ret = OperateDB::getInstance().handleAddFriend(caCurName, caTarName);
    qDebug() << "handleAddFriend: ret = " << ret;
    if (ret == 1)
    {
        // 转发给目标客户端
//        PDU * sendPdu = mkPDU(ENUM_MSG_TYPE_ADD_FRIEND_RESPOND, 0);
        MyTcpServer::getInstance().resend(caTarName, pdu);
    }
    PDU *respdu = mkPDU(ENUM_MSG_TYPE_ADD_FRIEND_RESPOND, 0);
    memcpy(respdu->caData, &ret, sizeof(ret));
    return respdu;
}

PDU *ReqHanler::handleAddFriendAgree()
{
    char caCurName[32] = {'\0'};
    char caTarName[32] = {'\0'};
    memcpy(caCurName, pdu->caData, 32);
    memcpy(caTarName, pdu->caData + 32, 32);
    qDebug() << "handleAddFriendAgree caName: " << caCurName
             << "caTarName: " << caTarName;
    bool ret = OperateDB::getInstance().handleAddFriendAgree(caCurName, caTarName);

    qDebug() << QString(caCurName) << " add : " << QString(caTarName) << " : " << ret;
    PDU *respdu = mkPDU(ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPOND, 0);
    memcpy(respdu->caData, &ret, sizeof(bool));
    MyTcpServer::getInstance().resend(caCurName, respdu);
    return respdu;
}

PDU *ReqHanler::handleFlushFriend()
{
    char caName[32] = "\0";
    memcpy(caName, pdu->caData, 32);

    qDebug() << "Flush UserName: " << caName;
    QStringList res = OperateDB::getInstance().handleFlushFriend(caName);
    qDebug() << "handleFlushFriend: res.size() = " << res.size();

    uint uiMsgLen = res.size() * 32;
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND, uiMsgLen);
    for (int i = 0; i < res.size(); ++i)
    {
        memcpy(pdu->caMsg + i * 32, res.at(i).toStdString().c_str(), 32);
    }
    return pdu;
}

PDU *ReqHanler::handleChat()
{
    char caTarName[32] = {'\0'};
    memcpy(caTarName, pdu->caData + 32, 32);
    qDebug() << "handleChat caTarName: " << caTarName;
    MyTcpServer::getInstance().resend(caTarName, pdu);
    return nullptr;
}

PDU *ReqHanler::handleMkdir()
{
    QString strCurPath = pdu->caMsg;
    char caDirName[1024];
    memcpy(caDirName, pdu->caData, 32);
    QString strNewDir = QString("%1/%2").arg(strCurPath).arg(caDirName);
    qDebug() << "strNewDir: " << strNewDir;
    QDir dir;
    bool ret = true;
    if (dir.exists(strNewDir))
    {
        ret = false;
    }
    else
    {
        ret = dir.mkdir(strNewDir);
    }
    qDebug() << "handleMkdir ret: " << ret;
    PDU *respdu = mkPDU(ENUM_MSG_TYPE_MKDIR_RESPOND, 0);
    memcpy(respdu->caData, &ret, sizeof(ret));
    return respdu;
}

PDU *ReqHanler::handleFlushFile()
{
    QDir dir(pdu->caMsg);
    QFileInfoList fileInfoList = dir.entryInfoList();
    QString strFileName;
    FileInfo *pFileInfo = nullptr;
    qDebug() << "handleFlushFile(): ";
    PDU *respdu = mkPDU(ENUM_MSG_TYPE_FLUSH_FILE_RESPOND, sizeof(FileInfo) * fileInfoList.size());
    for (int i = 0; i < fileInfoList.size(); i++)
    {
        strFileName = fileInfoList[i].fileName();

        pFileInfo = (FileInfo*)respdu->caMsg + i;
        memcpy(pFileInfo->caName, strFileName.toStdString().c_str(), 32);
        if (fileInfoList[i].isDir())
        {
            pFileInfo->iFileType = 0;
        }
        else if (fileInfoList[i].isFile())
        {
            pFileInfo->iFileType = 1;
        }
        qDebug() << strFileName << " iFileType: " << pFileInfo->iFileType;
    }
    return respdu;
}

PDU *ReqHanler::handleDelDir()
{
    QDir dir(pdu->caMsg);
    bool ret = dir.removeRecursively();
    PDU *respdu = mkPDU(ENUM_MSG_TYPE_DEL_DIR_RESPOND, 0);
    memcpy(respdu->caData, &ret, sizeof(ret));
    return respdu;
}

PDU *ReqHanler::handleMoveFile()
{
    int srcLen, tarLen;
    memcpy(&srcLen, pdu->caData, sizeof(int));
    memcpy(&tarLen, pdu->caData + 32, sizeof(int));
    char *srcFilePath = new char[srcLen + 1];
    char *tarFilePath = new char[tarLen + 1];

    memset(srcFilePath, '\0', srcLen + 1);
    memset(tarFilePath, '\0', tarLen + 1);

    memcpy(srcFilePath, pdu->caMsg, srcLen);
    memcpy(tarFilePath, pdu->caMsg + srcLen, tarLen);
    qDebug() << "handleMoveFile : " << srcFilePath << " -> " << tarFilePath;
    bool ret = QFile::rename(srcFilePath,tarFilePath);
    PDU *respdu = mkPDU(ENUM_MSG_TYPE_MV_FILE_RESPOND, 0);
    memcpy(respdu->caData, &ret, sizeof(ret));
    return respdu;
}

PDU *ReqHanler::handleUpLoadInit()
{
    qDebug() << "handleUpLoadInit()";
    // 取出文件名  文件大小
    qint64 iFileSize = 0;
    char caFileName[32] = {'\0'};
    memcpy(caFileName, pdu->caData, 32);
    memcpy(&iFileSize, pdu->caData + 32, sizeof(qint64));

    QString strFilePath = QString("%1/%2").arg(pdu->caMsg).arg(caFileName);
    qDebug() << "strFilePath: " << strFilePath;
    m_fUploadFile.setFileName(strFilePath);
    bool ret = false;
    if (m_fUploadFile.open(QIODevice::WriteOnly))
    {
        ret = true;
        m_iUploadFileSize = iFileSize;
        m_iUploadReceived = 0;
    }

    PDU *respdu = mkPDU(ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND, 0);
    memcpy(respdu->caData, &ret, sizeof(ret));
    return respdu;
}

PDU *ReqHanler::handleLoadFileData()
{
    // 讲文件写入文件对象
    m_fUploadFile.write(pdu->caMsg, pdu->uiMsgLen);
    // 更新接收文件大小
    m_iUploadReceived += pdu->uiMsgLen;
    // 判断文件大小
    if (m_iUploadReceived < m_iUploadFileSize)
    {
        return nullptr;
    }
    // 上传完成，关闭文件，相应给客户端
    m_fUploadFile.close();
    PDU *respdu = mkPDU(ENUM_MSG_TYPE_UPLOAD_FILE_DATA_RESPOND, 0);
    bool ret = m_iUploadReceived == m_iUploadFileSize;
    memcpy(respdu->caData, &ret, sizeof(ret));
    return respdu;
}

PDU *ReqHanler::hanleShareFile()
{
    // 取出好友数量
    int friendNum = 0;

    memcpy(&friendNum, pdu->caData + 32, sizeof(int));
    // 构建pdu，将当前用户名放入caData，分享文件路径放入caMSg
    PDU *resendpdu = mkPDU(pdu->uiMsgType, pdu->uiMsgLen - friendNum * 32);
    memcpy(resendpdu->caData, pdu->caData, 32);
    memcpy(resendpdu->caMsg, pdu->caMsg + friendNum * 32, pdu->uiMsgLen - friendNum * 32);

    // 遍历每一个好友，进行转发
    char caRecvName[32] = {'\0'};
    for (int i = 0; i < friendNum; ++i)
    {
        memcpy(caRecvName, pdu->caMsg + i * 32, 32);
        qDebug() << "caRecvName: " << caRecvName;
        MyTcpServer::getInstance().resend(caRecvName, resendpdu);
    }
    free(resendpdu);
    resendpdu = nullptr;

    // 响应给当前用户
    return mkPDU(ENUM_MSG_TYPE_SHARE_FILE_RESPOND, 0);
}

PDU *ReqHanler::hanleShareFileAgree()
{
    // 拼接分享到好友的路径
    QString strRecvPath = QString("%1/%2").arg(Server::getInstance().getRootDir()).arg(pdu->caData);
    QString strSrcPath = QString(pdu->caMsg);
    int index = strSrcPath.lastIndexOf('/');
    QString strFileName = strSrcPath.right(strSrcPath.size() - index - 1);
    strRecvPath = strRecvPath + "/" + strFileName;
    qDebug() << "srcPath: " <<strSrcPath << " ;" << "tarPath" << strRecvPath;

    bool ret = QFile::copy(strSrcPath, strRecvPath);
    qDebug() << "ret = " << ret;
    PDU *respdu = mkPDU(ENUM_MSG_TYPE_SHARE_FILE_AGREE_RESPOND, 0);
    memcpy(respdu->caData, &ret, sizeof(ret));
    return respdu;
}

