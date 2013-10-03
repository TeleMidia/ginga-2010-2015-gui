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
#include <QDateTime>
#include <QProcess>

#include "ui_qnplaboutform.h"

class QnplAboutDialog : public QDialog
{
  Q_OBJECT

public:
  QnplAboutDialog(QWidget* parent = 0);
  ~QnplAboutDialog();

public slots:
  void showMore();
  void printGingaVersion ();
  void printFailToStart(QProcess::ProcessError);

private:
  Ui::QnplAboutForm form;
  QProcess *process;
};

#endif // QNPLABOUTDIALOG_H
