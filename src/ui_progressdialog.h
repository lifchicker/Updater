/********************************************************************************
** Form generated from reading UI file 'progressdialog.ui'
**
** Created: Mon Sep 17 16:34:01 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROGRESSDIALOG_H
#define UI_PROGRESSDIALOG_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ProgressDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QProgressBar *progressBar;

    void setupUi(QDialog *ProgressDialog)
    {
        if (ProgressDialog->objectName().isEmpty())
            ProgressDialog->setObjectName(QString::fromUtf8("ProgressDialog"));
        ProgressDialog->resize(512, 70);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ProgressDialog->sizePolicy().hasHeightForWidth());
        ProgressDialog->setSizePolicy(sizePolicy);
        ProgressDialog->setMinimumSize(QSize(512, 70));
        ProgressDialog->setMaximumSize(QSize(512, 70));
        ProgressDialog->setContextMenuPolicy(Qt::NoContextMenu);
        ProgressDialog->setStyleSheet(QString::fromUtf8("/* \320\263\320\273\320\260\320\262\320\275\320\276\320\265 \320\276\320\272\320\275\320\276 */\n"
"QMenuBar,\n"
"QMainWindow {\n"
"        background-color: #e5e5e5;\n"
"}\n"
"QMenuBar {\n"
"        padding: 5px 0 0 5px;\n"
"}\n"
"\n"
"QProgressBar::chunk {\n"
"        background-color: #07a9eb;\n"
"        width: 10px;\n"
"        margin-right: 1px;\n"
"}"));
        ProgressDialog->setLocale(QLocale(QLocale::Russian, QLocale::RussianFederation));
        verticalLayout_2 = new QVBoxLayout(ProgressDialog);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(1, 1, 1, 1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(ProgressDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        progressBar = new QProgressBar(ProgressDialog);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(progressBar);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(ProgressDialog);

        QMetaObject::connectSlotsByName(ProgressDialog);
    } // setupUi

    void retranslateUi(QDialog *ProgressDialog)
    {
        ProgressDialog->setWindowTitle(QApplication::translate("ProgressDialog", "\320\237\321\200\320\276\320\263\321\200\320\265\321\201\321\201 \320\276\320\261\320\275\320\276\320\262\320\273\320\265\320\275\320\270\321\217", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ProgressDialog", "<html><head/><body><p><span style=\" font-size:10pt; font-weight:600;\">\320\237\320\276\320\264\320\276\320\266\320\264\320\270\321\202\320\265. \320\230\320\264\320\265\321\202 \320\277\321\200\320\276\321\206\320\265\321\201\321\201 \321\203\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\270 \320\275\320\276\320\262\320\276\320\271 \320\262\320\265\321\200\321\201\320\270\320\270.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ProgressDialog: public Ui_ProgressDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRESSDIALOG_H
