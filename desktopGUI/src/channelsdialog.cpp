#include "channelsdialog.h"
#include <QDebug>
#include <QModelIndex>

#include "mainwindow.h"

ChannelsDialog::ChannelsDialog(QWidget *parent)
  : QDialog(parent)
{
  MainWindow *mainWindow = qobject_cast <MainWindow *> (parent);

  if (mainWindow)
    connect (mainWindow, SIGNAL(scanFinished()), this, SLOT(loadGingaChannels()));

  setWindowTitle(tr("Listas de canais"));

  _formChannel.setupUi(this);

  _selectedRow = -1;

  // connecting
  connect(_formChannel.okButton, SIGNAL(clicked()), SLOT(accept()));
  connect(_formChannel.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(_formChannel.retuneButton, SIGNAL(clicked()), SLOT(scanChannels()));
  connect(_formChannel.frequencyCheckBox, SIGNAL(clicked()), SLOT (changeFrequencyVisibility()));
}

ChannelsDialog::~ChannelsDialog()
{
}

void ChannelsDialog::changeFrequencyVisibility()
{
  _formChannel.table->setColumnHidden(2, !_formChannel.table->isColumnHidden(2));
}

Channel ChannelsDialog::nextChannel()
{
  _selectedRow = (_selectedRow + 1) % _channels.count();
  return  _channels.at(_selectedRow);
}

Channel ChannelsDialog::previousChannel()
{
  if (--_selectedRow < 0)
    _selectedRow = _channels.count() - 1;

  return  _channels.at(_selectedRow);
}

void ChannelsDialog::loadGingaChannels()
{
  QString channelsFile = QDir::tempPath() + "/ginga/channels.txt";
  _channels.clear();

  if (QFile::exists(channelsFile)){
    QFile* file = new QFile(channelsFile);
    QTextStream in(file);

    if (file->open(QIODevice::ReadOnly)){
      while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList tokens = line.split(";");

        if(tokens.size()==4 && tokens[3] != "Unknown")
        {
          Channel channel;
          channel._frequency = tokens[1];
          channel._number = tokens[2];
          channel._name = tokens[3];

          _channels.push_back(channel);
        }
      }
      delete _formChannel.table->model();
      _formChannel.table->setModel(NULL);

      int ncol = 3;
      int nrow = 1;

      _formChannel.table->verticalHeader()->hide();
      _formChannel.table->horizontalHeader()->resizeSections(QHeaderView::Stretch);

      QStandardItemModel *model = new QStandardItemModel(nrow, ncol);


      model->setHorizontalHeaderItem(0, new QStandardItem("Name"));
      model->setHorizontalHeaderItem(1, new QStandardItem("Number"));
      model->setHorizontalHeaderItem(2, new QStandardItem("Frequency"));

      qSort(_channels);

      for(int i=0; i < _channels.count();i++)
      {
        QStandardItem* nitem = new QStandardItem(_channels[i]._name);
        nitem->setEditable(false);
        QStandardItem* vitem = new QStandardItem(_channels[i]._number);
        vitem->setEditable(false);
        QStandardItem* pitem = new QStandardItem(_channels[i]._frequency);
        pitem->setEditable(false);

        model->setItem(i, 0, nitem);
        model->setItem(i, 1, vitem);
        model->setItem(i, 2, pitem);
      }


      _formChannel.table->setModel(model);

      connect(_formChannel.table->selectionModel(),
              SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
              SLOT(printrow(QItemSelection,QItemSelection)));

      connect(_formChannel.table,
              SIGNAL(doubleClicked(QModelIndex)),
              SLOT(selectChannel(QModelIndex)));

      if (_channels.count()){
        _formChannel.table->selectRow(0);
        _selectedRow = 0;
      }

      _formChannel.table->setColumnHidden(2, !_formChannel.frequencyCheckBox->isChecked());
      _formChannel.table->resizeColumnsToContents();

    }

    delete file;
  }

  _formChannel.okButton->setEnabled(_channels.count() > 0);
}

void ChannelsDialog::selectChannel(QModelIndex selectedRow)
{
  int rowIndex = selectedRow.row();
  if (rowIndex < _channels.count()){
    this->_selectedRow = rowIndex;
    accept();
  }
}

Channel ChannelsDialog::channel()
{
  Channel c;
  if (_selectedRow >=0 && _selectedRow < _channels.count())
    c = _channels.at(_selectedRow);

  return c;
}

void ChannelsDialog::scanChannels()
{
  delete _formChannel.table->model();
  _formChannel.table->setModel(NULL);

  emit scanChannelsRequested();
}

void ChannelsDialog::printrow(QItemSelection itemSelection ,QItemSelection)
{
  if(!itemSelection.indexes().isEmpty())
  {
    if ( itemSelection.indexes().at(0).row()!= _selectedRow){
      _selectedRow = itemSelection.indexes().at(0).row();
    }
  }
  _formChannel.table->selectRow(_selectedRow);
}
