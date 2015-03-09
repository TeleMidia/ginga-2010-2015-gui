#ifndef CHANNELSDIALOG_H
#define CHANNELSDIALOG_H

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

#include "ui_channelsform.h"

class Channel {
public:
    QString frequency;
    QString number;
    QString name;

    inline bool operator <(const Channel c) const {
        return this->number.toDouble() < c.number.toDouble();
    }

    inline bool isNull () { return frequency == ""; }
    inline void setNull () { frequency = ""; number = ""; name = ""; }
};

class ChannelsDialog : public QDialog
{
    Q_OBJECT

 signals:
    void channelsimprimir(QString a,QString b,QString c);
    void channelsimprimirNext(QString a, QString b,QString c);
    void tunerChannelRequested (Channel);
    void scanChannelsRequested ();

public:
  ChannelsDialog(QWidget* parent = 0);
  ~ChannelsDialog();

  Channel nextChannel();
  Channel previousChannel();

  Channel channel ();

public slots:
   void scanChannels();
   void printrow(QItemSelection,QItemSelection);
   void selectChannel (QModelIndex);
   void changeFrequencyVisibility();
   void loadGingaChannels();

private:
    QStringList listanext;
    Ui::ChannelsForm formChannel;
    int selectedRow;
    QVector <Channel> channels;

};

#endif // CHANNELSDIALOG_H
