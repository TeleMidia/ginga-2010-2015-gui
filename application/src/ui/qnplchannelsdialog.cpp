#include "qnplchannelsdialog.h"
#include <QDebug>
#include <QModelIndex>
QnplChannelsDialog::QnplChannelsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Listas de canais"));

    formchannel.setupUi(this);




    // connecting
    connect(formchannel.Retune, SIGNAL(clicked()), SLOT(destroiGingaChannels()));


  }

QnplChannelsDialog::~QnplChannelsDialog()
{

}



void QnplChannelsDialog::  loadGingaChannels()
{

    QVector<QString> nome;
    QVector<QString> id;
    QVector<QString> frequencia;
    if (QFile::exists("/home/eduardo/teste")){

        QFile* file = new QFile("/home/eduardo/teste");
        QTextStream in(file);

        if (file->open(QIODevice::ReadOnly)){

            while(!in.atEnd()) {
                QString line = in.readLine();
                QStringList  aa = line.split(" ");


                if(aa.size()==3)
                {
                nome.push_back(aa[0]);
                id.push_back(aa[1]);
                frequencia.push_back(aa[2]);
                }
            }

            delete formchannel.table->model();
            formchannel.table->setModel(NULL);


            int ncol = 3;
            int nrow = 1;

            formchannel.table->verticalHeader()->hide();
            formchannel.table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

            QStandardItemModel *model = new QStandardItemModel(nrow, ncol);


            model->setHorizontalHeaderItem(0, new QStandardItem("Name"));
            model->setHorizontalHeaderItem(1, new QStandardItem("Id"));
            model->setHorizontalHeaderItem(2, new QStandardItem("frequencia"));



            for(int i=0;i<nome.size();i++)
              {
                    QStandardItem* nitem = new QStandardItem(nome[i]);
                    nitem->setEditable(false);
                    QStandardItem* vitem = new QStandardItem(id[i]);
                    vitem->setEditable(false);
                    QStandardItem* pitem = new QStandardItem(frequencia[i]);
                    pitem->setEditable(false);


                    model->setItem(i, 0, nitem);
                    model->setItem(i, 1, vitem);
                    model->setItem(i, 2, pitem);





               }


            formchannel.table->setModel(model);



            connect(formchannel.table->selectionModel(),
                                SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                                SLOT(printrow(QItemSelection,QItemSelection)));
        }

        delete file;
    }
}


void QnplChannelsDialog::  destroiGingaChannels()
{
    delete formchannel.table->model();
    formchannel.table->setModel(NULL);
    loadGingaChannels();
}

void  QnplChannelsDialog:: printrow(QItemSelection a ,QItemSelection b)
{
    QString texto1,texto2,texto3;
    QStandardItemModel *mymodel;

    mymodel=(QStandardItemModel*) formchannel.table->model();
    int linha=a.indexes().at(0).row();
    formchannel.table->selectRow(linha);



    texto1= mymodel->item(linha,0)->text();
    texto2= mymodel->item(linha,1)->text();
    texto3= mymodel->item(linha,2)->text();


    emit Channelsimprimir(texto1,texto2,texto3);

}
