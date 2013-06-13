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
#include <QStandardItemModel>
#include <QFileDialog>



#include "ui_qnplchannelsform.h"
#include "qnplsettings.h"

class QnplChannelsDialog : public QDialog
{
    Q_OBJECT

 signals:
    void Channelsimprimir(QString a,QString b,QString c);
    void ChannelsimprimirNext(QString a, QString b,QString c);

public:
  QnplChannelsDialog(QWidget* parent = 0);
  ~QnplChannelsDialog();
  void loadGingaChannels();
  QStringList getnext();



public slots:
   void destroiGingaChannels();
   void printrow(QItemSelection,QItemSelection);

private:
    QStringList listanext;
    Ui::QnplChannelsForm formchannel;
    int linhaselecionada;




};

#endif // QNPLCHANNELSDIALOG_H
