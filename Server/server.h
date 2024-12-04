#ifndef SERVER_H
#define SERVER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Server; }
QT_END_NAMESPACE

class Server : public QWidget
{
    Q_OBJECT

public:
    void loadConfig();
    static Server& getInstance();
    ~Server();
    QString getRootDir();
private:
    Ui::Server *ui;
    QString m_strIP;
    quint16 m_usPort;
    QString m_strRootDir;

    Server(QWidget *parent = nullptr);

};
#endif // SERVER_H
