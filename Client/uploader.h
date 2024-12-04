#ifndef UPLOADER_H
#define UPLOADER_H

#include "protocol.h"

#include <QObject>

class Uploader : public QObject
{
    Q_OBJECT
public:
    explicit Uploader(QObject *parent = nullptr);
    Uploader(const QString& filePath);
    void start();

    QString m_strUpLoadFilePath;
public slots:
    void uploadeFile();
signals:
    void errorHandle(const QString& error);
    void finished();
    void uploadPDU(PDU *pdu);

};

#endif // UPLOADER_H
