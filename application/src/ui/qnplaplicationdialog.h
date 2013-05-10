#ifndef QNPLAPLICATIONDIALOG_H
#define QNPLAPLICATIONDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QDateTime>

#include "ui_qnplaplicationform.h"

class QnplAplicationDialog : public QDialog
{
    Q_OBJECT

  public:
    QnplAplicationDialog(QWidget* parent = 0);
    ~QnplAplicationDialog();

private:
  Ui::Qnplaplicationform form;
};

#endif // QNPLAPLICATIONDIALOG_H
