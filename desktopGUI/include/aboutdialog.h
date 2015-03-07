#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

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

class AboutDialog : public QDialog
{
  Q_OBJECT

public:
  AboutDialog(QString execLocation, QWidget* parent = 0);
  ~AboutDialog();

public slots:
  void showMore();
  void printGingaVersion ();
  void printFailToStart(QProcess::ProcessError);

private:
  Ui::AboutForm form;
  QProcess *process;
};

#endif // ABOUTDIALOG_H
