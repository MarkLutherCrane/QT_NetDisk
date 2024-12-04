#ifndef FILE_H
#define FILE_H

#include "protocol.h"
#include "sharefile.h"
#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    QString m_strUserPath;
    QString m_strCurPath;
    QList<FileInfo*> m_pFileInfoList;
    QString m_strMoveFileName;
    QString m_strMoveFilePath;
    QString m_strUpLoadFilePath;
    ShareFile *m_pShareFile;
    QString m_strShareFilePath;


    explicit File(QWidget *parent = nullptr);
    ~File();
    void flushFile();
    void updateFileList(QList<FileInfo*> pFileList);
    bool isDir(QString strFileName);
    void upLoadFile();

private slots:
    void on_mkdir_PB_clicked();

    void on_flush_PB_clicked();

    void on_rmdir_PB_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_return_PB_clicked();

    void on_mv_PB_clicked();

    void on_upload_PB_clicked();

    void on_share_PB_clicked();

private:
    Ui::File *ui;
};

#endif // FILE_H
