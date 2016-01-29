#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

#include "ui_runform.h"

class ConfigDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ConfigDialog(QWidget *parent = 0);

signals:

public slots:
  void setGingaLocation ();
  void setContextLocation ();
  void customMenuRequested(QPoint);
  void removeVariable();

private slots:
  void loadSettings ();
  void saveSettings();

private:
  void saveGingaPreferences();

  Ui::RunForm *_runForm;
};

#endif // CONFIGDIALOG_H
