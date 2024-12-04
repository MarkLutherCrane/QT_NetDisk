/********************************************************************************
** Form generated from reading UI file 'file.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILE_H
#define UI_FILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_File
{
public:
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget;
    QGridLayout *gridLayout;
    QPushButton *upload_PB;
    QPushButton *flush_PB;
    QSpacerItem *verticalSpacer_3;
    QPushButton *download_PB;
    QPushButton *mv_PB;
    QSpacerItem *verticalSpacer_8;
    QPushButton *share_PB;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer_7;
    QSpacerItem *verticalSpacer_4;
    QPushButton *return_PB;
    QPushButton *rmdir_PB;
    QPushButton *mkdir_PB;
    QSpacerItem *verticalSpacer_5;
    QPushButton *rename_PB;
    QSpacerItem *verticalSpacer_6;
    QPushButton *remove_PB;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *File)
    {
        if (File->objectName().isEmpty())
            File->setObjectName(QString::fromUtf8("File"));
        File->resize(536, 473);
        horizontalLayout = new QHBoxLayout(File);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, -1, 0);
        listWidget = new QListWidget(File);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setFrameShape(QFrame::NoFrame);

        horizontalLayout->addWidget(listWidget);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        upload_PB = new QPushButton(File);
        upload_PB->setObjectName(QString::fromUtf8("upload_PB"));
        upload_PB->setMinimumSize(QSize(0, 50));
        QFont font;
        font.setFamily(QString::fromUtf8("Agency FB"));
        font.setPointSize(12);
        upload_PB->setFont(font);

        gridLayout->addWidget(upload_PB, 8, 1, 1, 1);

        flush_PB = new QPushButton(File);
        flush_PB->setObjectName(QString::fromUtf8("flush_PB"));
        flush_PB->setMinimumSize(QSize(0, 50));
        flush_PB->setFont(font);

        gridLayout->addWidget(flush_PB, 0, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(17, 68, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 3, 0, 1, 1);

        download_PB = new QPushButton(File);
        download_PB->setObjectName(QString::fromUtf8("download_PB"));
        download_PB->setMinimumSize(QSize(0, 50));
        download_PB->setFont(font);

        gridLayout->addWidget(download_PB, 8, 0, 1, 1);

        mv_PB = new QPushButton(File);
        mv_PB->setObjectName(QString::fromUtf8("mv_PB"));
        mv_PB->setMinimumSize(QSize(0, 50));
        mv_PB->setFont(font);

        gridLayout->addWidget(mv_PB, 6, 1, 1, 1);

        verticalSpacer_8 = new QSpacerItem(20, 68, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_8, 7, 1, 1, 1);

        share_PB = new QPushButton(File);
        share_PB->setObjectName(QString::fromUtf8("share_PB"));
        share_PB->setMinimumSize(QSize(0, 50));
        share_PB->setFont(font);

        gridLayout->addWidget(share_PB, 4, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(14, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 5, 0, 1, 1);

        verticalSpacer_7 = new QSpacerItem(14, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_7, 5, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(17, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_4, 1, 0, 1, 1);

        return_PB = new QPushButton(File);
        return_PB->setObjectName(QString::fromUtf8("return_PB"));
        return_PB->setMinimumSize(QSize(0, 50));
        return_PB->setFont(font);

        gridLayout->addWidget(return_PB, 0, 1, 1, 1);

        rmdir_PB = new QPushButton(File);
        rmdir_PB->setObjectName(QString::fromUtf8("rmdir_PB"));
        rmdir_PB->setMinimumSize(QSize(0, 50));
        rmdir_PB->setFont(font);

        gridLayout->addWidget(rmdir_PB, 4, 0, 1, 1);

        mkdir_PB = new QPushButton(File);
        mkdir_PB->setObjectName(QString::fromUtf8("mkdir_PB"));
        mkdir_PB->setMinimumSize(QSize(0, 50));
        mkdir_PB->setFont(font);

        gridLayout->addWidget(mkdir_PB, 2, 0, 1, 1);

        verticalSpacer_5 = new QSpacerItem(14, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_5, 1, 1, 1, 1);

        rename_PB = new QPushButton(File);
        rename_PB->setObjectName(QString::fromUtf8("rename_PB"));
        rename_PB->setMinimumSize(QSize(0, 50));
        rename_PB->setFont(font);

        gridLayout->addWidget(rename_PB, 2, 1, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 68, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_6, 3, 1, 1, 1);

        remove_PB = new QPushButton(File);
        remove_PB->setObjectName(QString::fromUtf8("remove_PB"));
        remove_PB->setMinimumSize(QSize(0, 50));
        remove_PB->setFont(font);

        gridLayout->addWidget(remove_PB, 6, 0, 1, 1);

        verticalSpacer = new QSpacerItem(17, 68, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 7, 0, 1, 1);


        horizontalLayout->addLayout(gridLayout);


        retranslateUi(File);

        QMetaObject::connectSlotsByName(File);
    } // setupUi

    void retranslateUi(QWidget *File)
    {
        File->setWindowTitle(QCoreApplication::translate("File", "Form", nullptr));
        upload_PB->setText(QCoreApplication::translate("File", "\344\270\212\344\274\240\346\226\207\344\273\266", nullptr));
        flush_PB->setText(QCoreApplication::translate("File", "\345\210\267\346\226\260\346\226\207\344\273\266", nullptr));
        download_PB->setText(QCoreApplication::translate("File", "\344\270\213\350\275\275\346\226\207\344\273\266", nullptr));
        mv_PB->setText(QCoreApplication::translate("File", "\347\247\273\345\212\250\346\226\207\344\273\266", nullptr));
        share_PB->setText(QCoreApplication::translate("File", "\345\210\206\344\272\253\346\226\207\344\273\266", nullptr));
        return_PB->setText(QCoreApplication::translate("File", "\350\277\224\345\233\236\344\270\212\344\270\200\347\272\247", nullptr));
        rmdir_PB->setText(QCoreApplication::translate("File", "\345\210\240\351\231\244\346\226\207\344\273\266\345\244\271", nullptr));
        mkdir_PB->setText(QCoreApplication::translate("File", "\345\210\233\345\273\272\346\226\207\344\273\266\345\244\271", nullptr));
        rename_PB->setText(QCoreApplication::translate("File", "\351\207\215\345\221\275\345\220\215", nullptr));
        remove_PB->setText(QCoreApplication::translate("File", "\345\210\240\351\231\244\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class File: public Ui_File {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILE_H
