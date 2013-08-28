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

class Channel {
public:
    QString frequency;
    QString number;
    QString name;

    inline bool operator <(const Channel c) const {
        return this->number.toDouble() < c.number.toDouble();
    }
};

class QnplChannelsDialog : public QDialog
{
    Q_OBJECT

 signals:
    void channelsimprimir(QString a,QString b,QString c);
    void channelsimprimirNext(QString a, QString b,QString c);
    void tunerChannelRequested (Channel);
    void scanChannelsRequested ();

public:
  QnplChannelsDialog(QWidget* parent = 0);
  ~QnplChannelsDialog();
  void loadGingaChannels(QString channelsFile);

  Channel nextChannel();
  Channel previousChannel();

  Channel channel ();

public slots:
   void scanChannels();
   void printrow(QItemSelection,QItemSelection);
   void selectChannel (QModelIndex);
   void changeFrequencyVisibility();

private:
    QStringList listanext;
    Ui::QnplChannelsForm formChannel;
    int selectedRow;
    QVector <Channel> channels;

};

#endif // QNPLCHANNELSDIALOG_H
