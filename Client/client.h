#ifndef CLIENT_H
#define CLIENT_H

#include "protocol.h"
#include "reshandler.h"
#include <QWidget>
#include <QTcpSocket>
QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QWidget
{
    Q_OBJECT

public:
    static Client& getInstance();
    ~Client();
    void loadConfig();
    void sendMsg(PDU* pdu);
    PDU* readMsg();
    void handleMsg(PDU *pdu);
    QString getRootDir();
    QString m_strLoginName;
    ResHandler *m_prh;
    QByteArray buffer;
    QTcpSocket m_tcpSocket;  // tcp对象


public slots:
    void showConnected();
    void recvMsg();
    void uploadError(const QString& error);

private slots:
    void on_regist_PB_clicked();
    void on_login_PB_clicked();

private:
    Ui::Client *ui;
    QString m_strIP;
    quint16 m_usPort;
    QString m_strRootDir;

    Client(QWidget *parent = nullptr);
    Client(const Client &instance) = delete;
    Client& operator=(const Client&) = delete;
    QTcpSocket& getSocket();
};
#endif // CLIENT_H
