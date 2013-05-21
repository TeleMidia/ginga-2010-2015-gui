#include "qnplchannelsdialog.h"
#include <QDebug>
QnplChannelsDialog::QnplChannelsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Listas de canais"));

    formchannel.setupUi(this);




    // connecting
    //connect(form.btMore, SIGNAL(clicked()), SLOT(showMore()));
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
        }

        delete file;
    }
}


void QnplChannelsDialog::  destroiGingaChannels()
{


    if (QFile::exists("/home/eduardo/teste")){

        QFile* file = new QFile("/home/eduardo/teste");


        if (file->open(QIODevice::ReadOnly)){

         delete formchannel.table->model();
            formchannel.table->setModel(model);
        }

        delete file;
    }
}
