#include "qnplchannelsdialog.h"
#include <QDebug>
#include <QModelIndex>
QnplChannelsDialog::QnplChannelsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Listas de canais"));

    formchannel.setupUi(this);

     linhaselecionada =-1;


    // connecting
    connect(formchannel.Retune, SIGNAL(clicked()), SLOT(destroiGingaChannels()));


  }

QnplChannelsDialog::~QnplChannelsDialog()
{

}


QStringList QnplChannelsDialog::  getnext()
{
    return  listanext;
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
            formchannel.table->horizontalHeader()->resizeSections(QHeaderView::Stretch);

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


    if(!a.indexes().isEmpty())
{
        if ( a.indexes().at(0).row()!=linhaselecionada){


    int i;
    i=formchannel.table->size().height();
    QString texto1,texto2,texto3,texto4,texto5,texto6;
    QStandardItemModel *mymodel;

    mymodel=(QStandardItemModel*) formchannel.table->model();
     linhaselecionada=a.indexes().at(0).row();

     if(linhaselecionada < i && linhaselecionada +1 < i ){
    texto1= mymodel->item(linhaselecionada,0)->text();
    texto2= mymodel->item(linhaselecionada,1)->text();
    texto3= mymodel->item(linhaselecionada,2)->text();

    texto4= mymodel->item(linhaselecionada +1,0)->text();
    texto5= mymodel->item(linhaselecionada +1,1)->text();
    texto6= mymodel->item(linhaselecionada +1,2)->text();
     listanext << texto4 << texto5 << texto6;
     }


    emit Channelsimprimir(texto1,texto2,texto3);



        }

}
      formchannel.table->selectRow(linhaselecionada);

}
