#include "server.h"

#include <QApplication>
#include "operatedb.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server::getInstance();

    OperateDB::getInstance().connect();
    return a.exec();
}
