#ifndef OPERATEDB_H
#define OPERATEDB_H

#include <QObject>
#include <QSqlDatabase>
class OperateDB : public QObject
{
    Q_OBJECT
public:
    QSqlDatabase m_db;

    ~OperateDB();
    static OperateDB& getInstance();
    void connect();
    // 处理注册
    bool handleResgist(const char* name, const char* pwd);
    bool handleLogin(const char* name, const char* pwd);
    bool handleOffine(const char* name);
    bool handleAllOffine();
    int handleFindUser(const char* name);
    int handleAddFriend(const char *curName, const char *tarName);
    bool handleAddFriendAgree(const char *curName, const char *tarName);
    QStringList handleOnlineUser();
    QStringList handleFlushFriend(const char* name);

private:
    explicit OperateDB(QObject *parent = nullptr);
    OperateDB(const OperateDB&) = delete;
    OperateDB& operator=(const OperateDB&) = delete;
signals:

};

#endif // OPERATEDB_H
