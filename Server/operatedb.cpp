#include "operatedb.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
OperateDB::~OperateDB()
{
    m_db.close();
}

OperateDB& OperateDB::getInstance()
{
    static OperateDB instance;
    return instance;
}

void OperateDB::connect()
{
    m_db.setHostName("localhost");
    m_db.setPort(3306);
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.setDatabaseName("mydb2401");

    if (m_db.open()) {
        qDebug() << "mysql connect success";
    } else {
        qDebug() << "mysql connect failed" << m_db.lastError().text();
    }
}

bool OperateDB::handleResgist(const char *name, const char *pwd)
{
    if (!name || !pwd)
    {
        return false;
    }
    QSqlQuery query;
    // 判断要添加的用户是否存在
    QString sql = QString("select * from user_info where name = '%1'").arg(name);
    qDebug() << "handleRegist::sql: " << sql;
    if (!query.exec(sql) || query.next())
    {
        return false;
    }

    // 添加用户
    sql = QString("insert into user_info (name, pwd) values ('%1', '%2')").arg(name).arg(pwd);
    qDebug() << "handleRegist::sql: " << sql;
    if (!query.exec(sql))
    {
        qDebug() << "Failed to insert data.";
        return false;
    }
    return true;
}

bool OperateDB::handleLogin(const char *name, const char *pwd)
{
    if (!name || !pwd)
    {
        return false;
    }
    QString sql = QString("select name, pwd, online from user_info where name = '%1' and pwd = '%2'").arg(name).arg(pwd);
    qDebug() << "handleLogin::sql: " << sql;
    QSqlQuery query(sql);
    if (!query.exec() || !query.next())
    {
        qDebug() << "name: " << name << " login failed";
        return false;
    }

    QString qname = query.value(0).toString(); // 获取第一列的值
    QString qpwd = query.value(1).toString();
    int qonline = query.value(2).toString().toUInt();
    qDebug() << "name: " << name << "pwd: " << pwd << "online: " << qonline;
    if (qname == QString(name) && qpwd == QString(pwd) && qonline == 0)
    {
        // 登录成功
        qDebug() << qname << "login success";
        qonline = 1;
        sql = QString("update user_info set online = 1 where name = '%1'").arg(name);
        if (!query.exec(sql)) {
            return false;
        }
        return true;
    } else {
        qDebug() << name << "already login";
    }

    return false;
}

bool OperateDB::handleOffine(const char *name)
{
    if (name == nullptr) return false;
    QString sql = QString("update user_info set online = 0 where name = '%1'").arg(name);
    qDebug() << "handleOffine::sql: " << sql;
    QSqlQuery query(sql);
    if (!query.exec(sql)) {
        return false;
    }
}

bool OperateDB::handleAllOffine()
{
    QString sql = QString("update user_info set online = 0");
    qDebug() << "handleAllOffine::sql: " << sql;
    QSqlQuery query(sql);
    if (!query.exec(sql)) {
        return false;
    }
}

int OperateDB::handleFindUser(const char *name)
{
    // -1 失败， 0离线， 1在线， 2不存在
    if (name == nullptr)
    {
        return -1;
    }
    QString sql = QString("select online from user_info where name = '%1'").arg(name);
    qDebug() << "handleFindUser::sql: " << sql;
    QSqlQuery query(sql);
    if (!query.exec(sql))
    {
        return -1;
    }
    if (query.next())
    {
        return query.value(0).toInt();
    }
    return 2;
}

int OperateDB::handleAddFriend(const char *curName, const char *tarName)
{
    if (!curName || !tarName)
    {
        return -1;  // 报错
    }
    QString sql = QString(R"(
                  select * from friend where
                  (
                      user_id = (select id from user_info where name = '%1')
                      and
                      friend_id = (select id from user_info where name = '%2')
                  )
                  or
                  (
                  user_id = (select id from user_info where name = '%2')
                      and
                      friend_id = (select id from user_info where name = '%1')

                  ))").arg(curName).arg(tarName);

    qDebug() << "handleAddFriend::sql: " << sql;
    QSqlQuery query;
    query.exec(sql);
    if (query.next())
    {
        return -2;  // 已经有好友关系
    }
    sql = QString("select online from user_info where name = '%1'").arg(tarName);
    query.exec(sql);
    if (query.next())
    {
        return query.value(0).toInt();
    }
    return -1;
}

bool OperateDB::handleAddFriendAgree(const char *curName, const char *tarName)
{
    if (!curName || !tarName)
    {
        return -1;  // 报错
    }
    QString sql = QString(R"(
                  insert into friend (user_id, friend_id)
                  select u1.id, u2.id from user_info u1, user_info u2 where u1.name = '%1' and u2.name = '%2'
                  )").arg(curName).arg(tarName);
    QSqlQuery query;
    qDebug() << "handleAddFriendAgree::sql: " << sql;

    return query.exec(sql);;
}

QStringList OperateDB::handleOnlineUser()
{
    QSqlQuery query;
    // 判断要添加的用户是否存在
    QString sql = QString("select name from user_info where online = 1");
    qDebug() << "handleOnlineUser::sql: " << sql;
    query.exec(sql);
    QStringList result;
    result.clear();
    while (query.next())
    {
        result.append(query.value(0).toString());
    }
    return result;
}

QStringList OperateDB::handleFlushFriend(const char *name)
{
    QSqlQuery query;
    QString sql = QString(R"(
          select name from user_info where online = 1 and
          id in
          (
          select friend_id from friend where user_id = (select id from user_info where name = '%1')
          union
          select user_id from friend where friend_id = (select id from user_info where name = '%1')
          )
         )").arg(name);
    qDebug() << "handleFlushFriend::sql: " << sql;
    query.exec(sql);
    QStringList result;
    result.clear();
    while (query.next())
    {
        result.append(query.value(0).toString());
    }
    return result;

}

OperateDB::OperateDB(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");

}
