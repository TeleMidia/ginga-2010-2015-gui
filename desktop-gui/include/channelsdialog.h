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
    QString _frequency;
    QString _number;
    QString _name;

    inline bool operator <(const Channel c) const {
        return this->_number.toDouble() < c._number.toDouble();
    }

    inline bool isNull () { return _frequency == ""; }
    inline void setNull () { _frequency = ""; _number = ""; _name = ""; }
};

class ChannelsDialog : public QDialog
{
    Q_OBJECT

 signals:
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
    QStringList _listNext;
    Ui::ChannelsForm _formChannel;
    int _selectedRow;
    QVector <Channel> _channels;

};

#endif // CHANNELSDIALOG_H
