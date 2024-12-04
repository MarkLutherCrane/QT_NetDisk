#include "index.h"
#include "client.h"
#include "protocol.h"
#include "sharefile.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client::getInstance().show();

//    Client w;
//    w.show();
    return a.exec();
}
