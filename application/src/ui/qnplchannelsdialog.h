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
#include "qnplsettings.h"

class QnplChannelsDialog : public QDialog
{
    Q_OBJECT

public:
  QnplChannelsDialog(QWidget* parent = 0);
  ~QnplChannelsDialog();
  void loadGingaChannels();

private:

  void loadSettings();
  Ui::QnplChannelsForm formchannel;




};

#endif // QNPLCHANNELSDIALOG_H
