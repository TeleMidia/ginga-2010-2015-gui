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

                qDebug() << aa[0];
                qDebug() << aa[1];
                qDebug() << aa[2];

                nome.push_back(aa[0]);
                id.push_back(aa[1]);
                frequencia.push_back(aa[2]);
            }











            // desenhar na tela
            /*
            int ncol = 3;
            int nrow = 32;

            formchannel.table->verticalHeader()->hide();
            formchannel.table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

            QStandardItemModel *model = new QStandardItemModel(nrow, ncol);

            model->setHorizontalHeaderItem(0, new QStandardItem("Name"));
            model->setHorizontalHeaderItem(1, new QStandardItem("Value"));

            int i = 0;

            QPair<QString, QString> p;
            foreach(p, params){
                if (p.first != "::" && p.first != "||"){
                    QStandardItem* nitem = new QStandardItem(p.first);
                    nitem->setEditable(true);

                    QStandardItem* vitem = new QStandardItem(p.second);

                    model->setItem(i, 0, nitem);
                    model->setItem(i, 1, vitem);

                    ++i;
                }
            }

            formchannel.table->setModel(model);*/
        }

        delete file;
    }
}
