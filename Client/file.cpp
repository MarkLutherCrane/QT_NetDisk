#include "file.h"
#include "ui_file.h"
#include "client.h"

#include <QIcon>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include <QFileDialog.h>

File::File(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File)
{
    ui->setupUi(this);
    m_strUserPath = QString("%1/%2")
                        .arg(Client::getInstance().getRootDir())
                        .arg(Client::getInstance().m_strLoginName);

    m_strCurPath = m_strUserPath;
    m_pShareFile = new ShareFile();
    flushFile();
}

File::~File()
{
    delete m_pShareFile;
    delete ui;
}

void File::flushFile()
{
    PDU *pdu = mkPDU(ENUM_MSG_TYPE_FLUSH_FILE_REQUEST, m_strCurPath.toStdString().size() + 1);
    memcpy(pdu->caMsg, m_strCurPath.toStdString().c_str(), m_strCurPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}

void File::updateFileList(QList<FileInfo *> pFileList)
{
    qDebug() << "updateFileList() start";
    // 每次更新前释放之前的文件信息。
    foreach(FileInfo *pFileInfo, m_pFileInfoList)
    {
        delete pFileInfo;
    }
    m_pFileInfoList = pFileList;
    ui->listWidget->clear();
    for (int i = 0; i < pFileList.size(); ++i)
    {
        QListWidgetItem *pItem = new QListWidgetItem;
        if (pFileList.at(i)->caName == QString(".") || pFileList.at(i)->caName == QString(".."))
        {
            continue;
        }
        pItem->setText(pFileList.at(i)->caName);
        if (pFileList.at(i)->iFileType == 0)
        {
            pItem->setIcon(QIcon(QPixmap(":/dir.png")));
        } else if (pFileList.at(i)->iFileType == 1)
        {
            pItem->setIcon(QIcon(QPixmap(":/file.png")));
        }
        ui->listWidget->addItem(pItem);
    }

}

bool File::isDir(QString strFileName)
{
    foreach(FileInfo *pFileInfo, m_pFileInfoList)
    {
        if (strFileName == pFileInfo->caName)
        {
            if (pFileInfo->iFileType != 0)
                return false;
            else return true;
        }
    }
    return false;
}

void File::upLoadFile()
{
    // 打算要上传的文件
    QFile file(m_strUpLoadFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "提示", "打开文件失败");
        return;
    }
    // 构建pdu, 每次4096
    PDU *pdu = mkPDU(ENUM_MSG_TYPE_UPLOAD_FILE_DATA_REQUEST, 4096);
    // 循环发送
    while(true)
    {
        int ret = file.read(pdu->caMsg, 4096);
        if (ret == 0)
        {
            break;
        }
        if (ret < 0)
        {
            QMessageBox::warning(this, "提示", "读取文件失败");
            break;
        }

        // 更新长度并发送
        pdu->uiMsgLen = ret;
        pdu->uiPDULen = ret + sizeof(PDU);
        Client::getInstance().m_tcpSocket.write((char*)pdu, pdu->uiPDULen);
//        qDebug() << "send uiMsgType: " << pdu->uiMsgType
//                 << "uiPDULen: " << pdu->uiPDULen
//                 << "uiMsgLen" << pdu->uiMsgLen
//                 << "caData: " << pdu->caData
//                << "caData + 32: " << pdu->caData + 32
//                 << "caMsg: " << pdu->caMsg;
    }

    file.close();
    free(pdu);
    pdu = nullptr;
}


void File::on_mkdir_PB_clicked()
{
    QString strNewDir = QInputDialog::getText(this, "新建文件夹", "新建文件夹名");
    if (strNewDir.isEmpty() || strNewDir.toStdString().size() > 32)
    {
        QMessageBox::information(this, "提示", "文件夹名长度非法");
        return;
    }
    PDU *pdu = mkPDU(ENUM_MSG_TYPE_MKDIR_REQUEST, m_strCurPath.toStdString().size() + 1);
    memcpy(pdu->caData, strNewDir.toStdString().c_str(), 32);
    memcpy(pdu->caMsg, m_strCurPath.toStdString().c_str(), m_strCurPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);

}

void File::on_flush_PB_clicked()
{
    flushFile();
}

void File::on_rmdir_PB_clicked()
{
    QListWidgetItem *pItem =ui->listWidget->currentItem();
    if (pItem == NULL)
    {
        QMessageBox::information(this, "提示", "请选择要删除的文件夹");
        return;
    }
    QString strFileName = pItem->text();
    foreach(FileInfo *pFileInfo, m_pFileInfoList)
    {
        if (strFileName == pFileInfo->caName && pFileInfo->iFileType != 0)
        {
            QMessageBox::information(this, "提示", "选择的不是文件夹");
            return;
        }
    }
    QString strPath = QString("%1/%2").arg(m_strCurPath).arg(strFileName);
    PDU* pdu = mkPDU(ENUM_MSG_TYPE_DEL_DIR_REQUEST, strPath.toStdString().size());
    memcpy(pdu->caMsg, strPath.toStdString().c_str(), strPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}

void File::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString strFileName = item->text();
    foreach(FileInfo *pFileInfo, m_pFileInfoList)
    {
        if (strFileName == pFileInfo->caName && pFileInfo->iFileType != 0)
        {

            return;
        }
    }

    m_strCurPath = QString("%1/%2").arg(m_strCurPath).arg(item->text());
    flushFile();
}

void File::on_return_PB_clicked()
{
    if (m_strCurPath == m_strUserPath)
    {
        return ;
    }
    int index = m_strCurPath.lastIndexOf('/');
    // toStdString.size() 算出来是字节， 所以应该直接.size(), 一个中文站3个字节
    m_strCurPath.remove(index, m_strCurPath.size() - index);
    flushFile();
}

void File::on_mv_PB_clicked()
{
    if (ui->mv_PB->text() == "移动文件")
    {
        QListWidgetItem *pItem = ui->listWidget->currentItem();
        if (pItem == NULL)
        {
            QMessageBox::information(this, "提示", "请选择要移动的文件夹");
            return;
        }
        QMessageBox::information(this, "提示", "请选择要移动到的目录");
        m_strMoveFileName = pItem->text();
        m_strMoveFilePath = m_strCurPath + '/' + m_strMoveFileName;

        ui->mv_PB->setText("确认/取消");
        return;
    }
    ui->mv_PB->setText("移动文件");
    QListWidgetItem *pItem = ui->listWidget->currentItem();
    if (pItem == NULL)
    {
        QMessageBox::information(this, "提示", "请选择要移动的文件夹");
        return;
    }
    if (!isDir(pItem->text()))
    {
        QMessageBox::information(this, "提示", "选择的文件不是目录");
        return;
    }
    int ret = QMessageBox::information(this, "提示", "是否确认移动到该目录下", "确认", "取消");
    if (ret == 1)
    {
        return ;
    }
    QString strTarPath = m_strCurPath + '/' + pItem->text() + '/' + m_strMoveFileName ;
    int srcLen = m_strMoveFilePath.toStdString().size();
    int tarLen = strTarPath.toStdString().size();
    PDU *pdu = mkPDU(ENUM_MSG_TYPE_MV_FILE_REQUEST, srcLen + tarLen + 1);
    memcpy(pdu->caData, &srcLen, 32);
    memcpy(pdu->caData + 32, &tarLen,32);
    memcpy(pdu->caMsg, m_strMoveFilePath.toStdString().c_str(), srcLen);
    memcpy(pdu->caMsg + srcLen, strTarPath.toStdString().c_str(), tarLen);

    Client::getInstance().sendMsg(pdu);
}

void File::on_upload_PB_clicked()
{
    // 通过文件选择弹窗选择文件
    m_strUpLoadFilePath.clear();
    m_strUpLoadFilePath = QFileDialog::getOpenFileName();
    if (m_strUpLoadFilePath.isEmpty()) { return; }
    qDebug () << "m_strUpLoadFilePath: " << m_strUpLoadFilePath;

    // 截断取文件名
    int idx = m_strUpLoadFilePath.lastIndexOf('/');
    QString strFileName = m_strUpLoadFilePath.right(m_strUpLoadFilePath.size() - idx - 1);
    qDebug() << "strFileName: " << strFileName;

    // 打开文件，查看文件大小
    QFile qFile(m_strUpLoadFilePath);
    qint64 iFileSize = qFile.size();
    qDebug () << "qFile size = " << iFileSize;

    // caData: 文件名 + 文件大小; caMsg: 文件存储路径。
    PDU *pdu = mkPDU(ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST, m_strCurPath.toStdString().size() + 1);
    memcpy(pdu->caData, strFileName.toStdString().c_str(), 32);
    memcpy(pdu->caData + 32, &iFileSize, sizeof(qint64));
    memcpy(pdu->caMsg, m_strCurPath.toStdString().c_str(), m_strCurPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}

void File::on_share_PB_clicked()
{
    QListWidgetItem *pItem = ui->listWidget->currentItem();
    if (pItem == nullptr)
    {
        QMessageBox::information(this, "提示", "请选择要分享的文件");
        return;
    }
    m_strShareFilePath =QString("%1/%2").arg(m_strCurPath).arg(pItem->text());

    m_pShareFile->updateFriend_LW();
    if (m_pShareFile->isHidden())
    {
        m_pShareFile->show();
    }
}
