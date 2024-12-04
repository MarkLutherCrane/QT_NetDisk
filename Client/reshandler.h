#ifndef RESHANDLER_H
#define RESHANDLER_H

#include "protocol.h"

#include <QObject>

class ResHandler : public QObject
{
    Q_OBJECT
public:
    explicit ResHandler(QObject *parent = nullptr);
    PDU* pdu;
    void handleRegist();
    void handleLogin();
    void handleFindUser();
    void handleOnlineUser();
    void handleAddFriend();
    void handleAddFriendResend();
    void handleAddFriendAgree();
    void handleFlushFriend();
    void handleChatResend();
    void handleChatMkdir();
    void handleFlushFile();
    void handleDelDir();
    void handleMoveFile();
    void handleUploadFileInit();
    void handleUploadData();
    void handleShareFileResend();
    void handleShareFileAgree();
signals:

};

#endif // RESHANDLER_H
