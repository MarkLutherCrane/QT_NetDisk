#ifndef INDEX_H
#define INDEX_H

#include "file.h"
#include "friend.h"

#include <QWidget>

namespace Ui {
class Index;
}

class Index : public QWidget
{
    Q_OBJECT

public:
    static Index &getInstance();
    ~Index();
    Friend* getFriend();
    File* getFile();

private slots:
    void on_friend_PB_clicked();
    void on_file_PB_clicked();

private:
    Ui::Index *ui;
    explicit Index(QWidget *parent = nullptr);
    Index(const Index&) = delete;
    Index& operator=(const Index&) = delete;
};

#endif // INDEX_H
