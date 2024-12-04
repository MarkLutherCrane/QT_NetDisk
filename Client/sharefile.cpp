#include "sharefile.h"
#include "ui_sharefile.h"
#include "index.h"
#include "client.h"
ShareFile::ShareFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShareFile)
{
    ui->setupUi(this);
}

ShareFile::~ShareFile()
{
    delete ui;
}

void ShareFile::updateFriend_LW()
{
    ui->listWidget->clear();
    QListWidget* friend_LW = Index::getInstance().getFriend()->getFriend_LW();
    for (int i = 0; i < friend_LW->count(); ++i)
    {
        QListWidgetItem *friendItem = friend_LW->item(i);
        QListWidgetItem *newItem =new QListWidgetItem(*friendItem);
        ui->listWidget->addItem(newItem);
    }
}

void ShareFile::on_allSelected_PB_2_clicked()
{
    for (int i = 0; i < ui->listWidget->count(); ++i)
    {
        ui->listWidget->item(i)->setSelected(true);
    }
}

void ShareFile::on_cancelSelected_PB_clicked()
{
    for (int i = 0; i < ui->listWidget->count(); ++i)
    {
        ui->listWidget->item(i)->setSelected(false);
    }
}

void ShareFile::on_ok_PB_clicked()
{
    // 取出列表框中的好友，并计算其数量
    QList<QListWidgetItem*> pItems = ui->listWidget->selectedItems();
    int friendNum = pItems.size();
    // 取出文件分享的路径
    QString strPath = Index::getInstance().getFile()->m_strShareFilePath;
    qDebug() << "on_ok_PB_clicked: " << strPath;
    // 构建pdu，当前用户名和好友数量放入caData, 好友名和分享文件路径放入caMsg
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_SHARE_FILE_REQUEST, pItems.size() * 32 + strPath.toStdString().size() + 1);
    memcpy(pdu->caData, Client::getInstance().m_strLoginName.toStdString().c_str(), 32);
    memcpy(pdu->caData + 32, &friendNum, sizeof(int));
    for (int i = 0; i < friendNum; ++i)
    {
        memcpy(pdu->caMsg + i * 32, pItems.at(i)->text().toStdString().c_str(), 32);
    }
    memcpy(pdu->caMsg + friendNum * 32,strPath.toStdString().c_str(), strPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}
