#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

#include "ui_aboutform.h"

class AboutDialog : public QDialog
{
  Q_OBJECT

public:
  AboutDialog(QString execLocation, QWidget* parent = 0);
  ~AboutDialog() {}

public slots:
  void showMore();

private:
  Ui::AboutForm _form;  
};

#endif // ABOUTDIALOG_H
