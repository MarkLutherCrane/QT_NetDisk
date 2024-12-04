#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include "chat.h"
#include "onlineuser.h"
namespace Ui {
class Friend;
}

class Friend : public QWidget
{
    Q_OBJECT

public:
    explicit Friend(QWidget *parent = nullptr);
    ~Friend();
    void flushFriend();
    void updateListWidget(QStringList nameList);
    QListWidget* getFriend_LW();

    OnlineUser *m_pOnlineUser;
    Chat *m_pChat;
private slots:
    void on_findUser_PB_clicked();

    void on_onlineUser_PB_clicked();

    void on_flush_PB_clicked();

    void on_chat_PB_clicked();

private:
    Ui::Friend *ui;

};

#endif // FRIEND_H
