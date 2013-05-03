#ifndef QNPLCHANNELSDIALOG_H
#define QNPLCHANNELSDIALOG_H

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

class QnplChannelsDialog : public QDialog
{
    Q_OBJECT

public:
  QnplChannelsDialog(QWidget* parent = 0);
  ~QnplChannelsDialog();

private:
  Ui::QnplChannelsForm form;
};

#endif // QNPLCHANNELSDIALOG_H
