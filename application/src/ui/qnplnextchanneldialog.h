#ifndef QNPLNEXTCHANNELDIALOG_H
#define QNPLNEXTCHANNELDIALOG_H


#include <QDialog>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QDateTime>

#include "ui_qnplchannelsform.h"
#include "qnplsettings.h"


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






#endif // QNPLNEXTCHANNELDIALOG_H
