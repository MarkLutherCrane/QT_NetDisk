#include "uploader.h"

#include <QFile>
#include <QMessageBox>
#include <QMessageBox>
#include <QThread>

Uploader::Uploader(QObject *parent) : QObject(parent)
{

}

Uploader::Uploader(const QString &filePath) : m_strUpLoadFilePath(filePath)
{

}

void Uploader::uploadeFile()
{
    // 打算要上传的文件
    QFile file(m_strUpLoadFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        emit errorHandle("打开文件失败");
        emit finished();
        return;
    }
    // 构建pdu, 每次4096

    // 循环发送
    while(true)
    {
        PDU *pdu = mkPDU(ENUM_MSG_TYPE_UPLOAD_FILE_DATA_REQUEST, 4096);
        int ret = file.read(pdu->caMsg, 4096);
        if (ret == 0)
        {
            break;
        }
        if (ret < 0)
        {
            emit errorHandle("读取文件失败");
            break;
        }

        // 更新长度并发送
        pdu->uiMsgLen = ret;
        pdu->uiPDULen = ret + sizeof(PDU);
        emit uploadPDU(pdu);
    }

    file.close();
    emit finished();
}

void Uploader::start()
{
    QThread* thread = new QThread();

    this->moveToThread(thread);
    // 关联线程对象的开始 和 upload的开始, upload的结束和线程对象的quit结束
    connect(thread, &QThread::started, this, &Uploader::uploadeFile);
    connect(this, &Uploader::finished, thread, &QThread::quit);

    // 默认操作
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    // 开启线程
    thread->start();
}
