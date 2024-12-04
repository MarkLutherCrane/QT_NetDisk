#include "friend.h"
#include "ui_friend.h"
#include "client.h"
#include "protocol.h"
#include <QInputDialog>
#include <qmessagebox.h>

Friend::Friend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Friend)
{
    ui->setupUi(this);
    m_pOnlineUser = new OnlineUser();
    m_pChat = new Chat();
    flushFriend();
}

Friend::~Friend()
{
    delete m_pOnlineUser;
    delete m_pChat;
    delete ui;
}

void Friend::flushFriend()
{
    PDU *pdu = mkPDU(ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST, 0);
    memcpy(pdu->caData, Client::getInstance().m_strLoginName.toStdString().c_str(), 32);
    Client::getInstance().sendMsg(pdu);
}

void Friend::updateListWidget(QStringList nameList)
{
    ui->listWidget->clear();
    ui->listWidget->addItems(nameList);
}

QListWidget* Friend::getFriend_LW()
{
    return ui->listWidget;
}

void Friend::on_findUser_PB_clicked()
{
    QString strName = QInputDialog::getText(this, "查找用户", "用户名：");
    qDebug() << "findUser Name: " << strName;
    if (strName.isEmpty())
    {
        return;
    }
    PDU *pdu = mkPDU(ENUM_MSG_TYPE_FIND_USER_REQUEST, 0);
    memcpy(pdu->caData, strName.toStdString().c_str(), 32);
    Client::getInstance().sendMsg(pdu);
}

void Friend::on_onlineUser_PB_clicked()
{
    if (m_pOnlineUser->isHidden())
    {
        m_pOnlineUser->show();
    }
    PDU *pdu = mkPDU(ENUM_MSG_TYPE_ONLINE_USER_REQUEST, 0);
    Client::getInstance().sendMsg(pdu);
}

void Friend::on_flush_PB_clicked()
{
    flushFriend();
}

void Friend::on_chat_PB_clicked()
{
    QListWidgetItem *pItem = ui->listWidget->currentItem();
    if (!pItem)
    {
        QMessageBox::information(this, "提示", "请选择要聊天的好友");
        return;
    }
    m_pChat->m_strChatName = pItem->text();
    m_pChat->setWindowTitle(m_pChat->m_strChatName);
    if (m_pChat->isHidden())
    {
        m_pChat->show();
    }
}
