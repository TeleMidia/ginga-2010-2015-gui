#ifndef QNPLABOUTDIALOG_H
#define QNPLABOUTDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

#include "ui_QnplAboutForm.h"

class QnplAboutDialog : public QDialog
{
    Q_OBJECT

public:
    QnplAboutDialog(QWidget* parent = 0);
    ~QnplAboutDialog();

public slots:
    void showMore();

private:
    Ui::QnplAboutForm form;
};

#endif // QNPLABOUTDIALOG_H
