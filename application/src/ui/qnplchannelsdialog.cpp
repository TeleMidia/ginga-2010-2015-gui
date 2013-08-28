#include "qnplchannelsdialog.h"
#include <QDebug>
#include <QModelIndex>

#include "qnplmainwindow.h"

QnplChannelsDialog::QnplChannelsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Listas de canais"));

    formChannel.setupUi(this);

    selectedRow =-1;

    // connecting
    connect(formChannel.okButton, SIGNAL(clicked()), SLOT(accept()));
    connect(formChannel.cancelButton, SIGNAL(clicked()), SLOT(reject()));
    connect(formChannel.retuneButton, SIGNAL(clicked()), SLOT(scanChannels()));
    connect(formChannel.frequencyCheckBox, SIGNAL(clicked()), SLOT (changeFrequencyVisibility()));
}

QnplChannelsDialog::~QnplChannelsDialog()
{

}

void QnplChannelsDialog::changeFrequencyVisibility()
{
    formChannel.table->setColumnHidden(2, !formChannel.table->isColumnHidden(2));
}

Channel QnplChannelsDialog::nextChannel()
{
    selectedRow = (selectedRow + 1) % channels.count();
    return  channels.at(selectedRow);
}

Channel QnplChannelsDialog::previousChannel()
{
    selectedRow = (--selectedRow < 0) ? selectedRow = channels.count() - 1 : selectedRow;
    return  channels.at(selectedRow);
}

void QnplChannelsDialog::  loadGingaChannels(QString channelsFile)
{
    channels.clear();

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
                    channel.frequency = tokens[1];
                    channel.number = tokens[2];
                    channel.name = tokens[3];

                    channels.push_back(channel);
                }
            }
            delete formChannel.table->model();
            formChannel.table->setModel(NULL);

            int ncol = 3;
            int nrow = 1;

            formChannel.table->verticalHeader()->hide();
            formChannel.table->horizontalHeader()->resizeSections(QHeaderView::Stretch);

            QStandardItemModel *model = new QStandardItemModel(nrow, ncol);


            model->setHorizontalHeaderItem(0, new QStandardItem("Name"));
            model->setHorizontalHeaderItem(1, new QStandardItem("Number"));
            model->setHorizontalHeaderItem(2, new QStandardItem("Frequency"));

            qSort(channels);

            for(int i=0; i < channels.count();i++)
            {
                QStandardItem* nitem = new QStandardItem(channels[i].name);
                nitem->setEditable(false);
                QStandardItem* vitem = new QStandardItem(channels[i].number);
                vitem->setEditable(false);
                QStandardItem* pitem = new QStandardItem(channels[i].frequency);
                pitem->setEditable(false);

                model->setItem(i, 0, nitem);
                model->setItem(i, 1, vitem);
                model->setItem(i, 2, pitem);
            }


            formChannel.table->setModel(model);

            connect(formChannel.table->selectionModel(),
                    SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                    SLOT(printrow(QItemSelection,QItemSelection)));

            connect(formChannel.table,
                    SIGNAL(doubleClicked(QModelIndex)),
                    SLOT(selectChannel(QModelIndex)));

            if (channels.count()){
                formChannel.table->selectRow(0);
                selectedRow = 0;
            }

            formChannel.table->setColumnHidden(2, true);
        }

        delete file;
    }
}

void QnplChannelsDialog::selectChannel(QModelIndex selectedRow)
{
    int rowIndex = selectedRow.row();
    if (rowIndex < channels.count()){
        this->selectedRow = rowIndex;
        accept();
    }
}

Channel QnplChannelsDialog::channel()
{
    Channel c;
    if (selectedRow >=0 && selectedRow < channels.count())
        c = channels.at(selectedRow);

    return c;
}

void QnplChannelsDialog::scanChannels()
{
    delete formChannel.table->model();
    formChannel.table->setModel(NULL);

    emit scanChannelsRequested();
}

void  QnplChannelsDialog:: printrow(QItemSelection itemSelection ,QItemSelection)
{
    if(!itemSelection.indexes().isEmpty())
    {
        if ( itemSelection.indexes().at(0).row()!= selectedRow){
            QStandardItemModel *mymodel;

            mymodel=(QStandardItemModel*) formChannel.table->model();
            selectedRow = itemSelection.indexes().at(0).row();
        }

    }
    formChannel.table->selectRow(selectedRow);
}
