#ifndef REQHANLER_H
#define REQHANLER_H

#include "protocol.h"

#include <QObject>
#include <qfile.h>

class ReqHanler : public QObject
{
    Q_OBJECT
public:
    explicit ReqHanler(QObject *parent = nullptr);
    PDU *pdu;
    QFile m_fUploadFile;
    qint64 m_iUploadFileSize;
    qint64 m_iUploadReceived;

    PDU *handleRegist();
    PDU *handleLogin(QString& loginName);
    PDU *handleFindUser();
    PDU *handleOnlineUser();
    PDU *handleAddFriend();
    PDU *handleAddFriendAgree();
    PDU *handleFlushFriend();
    PDU *handleChat();
    PDU *handleMkdir();
    PDU *handleFlushFile();
    PDU *handleDelDir();
    PDU *handleMoveFile();
    PDU *handleUpLoadInit();
    PDU *handleLoadFileData();
    PDU *hanleShareFile();
    PDU *hanleShareFileAgree();

signals:

};

#endif // REQHANLER_H
