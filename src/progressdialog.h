#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>

#include "ui_progressdialog.h"

class QProgressBar;

class ProgressDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ProgressDialog(QWidget *parent = 0);
    ~ProgressDialog();
    
    const QProgressBar * getProgressBar();

    //public slots:
//    void updateProgress(int value);

private:
    Ui::ProgressDialog *ui;
};

#endif // PROGRESSDIALOG_H
