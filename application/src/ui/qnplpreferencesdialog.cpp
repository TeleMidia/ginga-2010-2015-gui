#include "qnplpreferencesdialog.h"

#include <QDebug>

QnplPreferencesDialog::QnplPreferencesDialog(QWidget* parent)
    : QDialog(parent)
{

    wGeneral = new QWidget();
    wRun = new QWidget();

    formGeneral.setupUi(new QDialog(this));
    formRun.setupUi(new QDialog(this));

    wGeneral->setLayout(formGeneral.verticalLayout);
    wRun->setLayout(formRun.verticalLayout);

    formPreferences.setupUi(this);


    connect(formPreferences.dTree, SIGNAL(clicked(QModelIndex)), SLOT(showPreferencesItem(QModelIndex)));
    connect(formPreferences.dButtons, SIGNAL(accepted()), SLOT(saveSettings()));
    connect(formRun.pushButton,SIGNAL(clicked()), SLOT(browseExecutable()));
    connect(formRun.pushButton_2,SIGNAL(clicked()), SLOT(browseGingaSettingsFile()));
}

QnplPreferencesDialog::~QnplPreferencesDialog()
{
    delete wGeneral;
    delete wRun;
}

void QnplPreferencesDialog::init(QnplSettings* s)
{
    settings = s;

    QStandardItemModel* model = new QStandardItemModel(2,1);
    QStandardItem* g = new QStandardItem(tr("General"));
    g->setEditable(false);
    g->setIcon(QIcon(":icon/preferences"));
    QStandardItem* r = new QStandardItem(tr("Run"));
    r->setEditable(false);
    r->setIcon(QIcon(":icon/run"));

    model->setItem(0, g);
    model->setItem(1, r);


    formPreferences.dTree->setModel(model);
    formPreferences.dTree->selectionModel()->select(formPreferences.dTree->model()->index(0,0), QItemSelectionModel::Select);
    formPreferences.dTree->header()->hide();


    formPreferences.dLabel->setText(tr("General"));
    formPreferences.dLabel->setStyleSheet("font-weight: bold; font-size 32px;");


    formPreferences.verticalLayout->addWidget(wGeneral);
    formPreferences.verticalLayout->addWidget(wRun);
    wGeneral->show();
    wRun->hide();

    loadSettings();
}

void QnplPreferencesDialog::loadSettings()
{
    if (settings->value("autoplay").toString() == "true"){
        formGeneral.checkBox->setChecked(true);
    }else{
        formGeneral.checkBox->setChecked(false);
    }

    formRun.lineEdit->setText(settings->value("location").toString());

    if (settings->value("screensize").toString() == "640x480"){
        formRun.comboBox->setCurrentIndex(0);
    }else if (settings->value("screensize").toString() == "800x600"){
        formRun.comboBox->setCurrentIndex(1);
    }else if (settings->value("screensize").toString() == "1024x768"){
        formRun.comboBox->setCurrentIndex(2);
    }else if (settings->value("screensize").toString() == "854x480"){
        formRun.comboBox->setCurrentIndex(3);
    }else if (settings->value("screensize").toString() == "1280x720"){
        formRun.comboBox->setCurrentIndex(4);
    }else if (settings->value("screensize").toString() == "1920x1080"){
        formRun.comboBox->setCurrentIndex(5);
    }else if (settings->value("screensize").toString() == "320x400"){
        formRun.comboBox->setCurrentIndex(8); // (!) index 6 is an empty option
    }else if (settings->value("screensize").toString() == "400x320"){
        formRun.comboBox->setCurrentIndex(9);
    }else if (settings->value("screensize").toString() == "320x180"){
        formRun.comboBox->setCurrentIndex(10);
    }else if (settings->value("screensize").toString() == "320x240"){
        formRun.comboBox->setCurrentIndex(11);
    }else if (settings->value("screensize").toString() == "240x320"){
        formRun.comboBox->setCurrentIndex(12);
    }else if (settings->value("screensize").toString() == "fullscreen"){
        formRun.comboBox->setCurrentIndex(6);
    }

    if (settings->value("lang").toString() == "en"){
        formGeneral.comboBox->setCurrentIndex(0);
    }else if (settings->value("lang").toString() == "pt_br"){
        formGeneral.comboBox->setCurrentIndex(1);
    }else if (settings->value("lang").toString() == "en"){
        formGeneral.comboBox->setCurrentIndex(2);
    }

    formRun.textEdit->setText(settings->value("parameters").toString());

    formRun.lineEdit_2->setText(settings->value("gingaconfig_file").toString());

    loadGingaPreferences();
}

void QnplPreferencesDialog::saveSettings()
{
    qDebug() << "Saving settings...";

    if (formGeneral.checkBox->isChecked()){
        settings->setValue("autoplay",true);
    }else{
        settings->setValue("autoplay",false);
    }

    settings->setValue("location",formRun.lineEdit->text());

    switch (formRun.comboBox->currentIndex()){
    case 0:
        settings->setValue("screensize","640x480");
        break;
    case 1:
        settings->setValue("screensize","800x600");
        break;
    case 2:
        settings->setValue("screensize","1024x768");
        break;
    case 3:
        settings->setValue("screensize","854x480");
        break;
    case 4:
        settings->setValue("screensize","1280x720");
        break;
    case 5:
        settings->setValue("screensize","1920x1080");
        break;
	case 6:
        settings->setValue("screensize","fullscreen");
        break;
    case 8:
        settings->setValue("screensize","320x400");
        break;
    case 9:
        settings->setValue("screensize","400x320");
        break;
    case 10:
        settings->setValue("screensize","320x180");
        break;
    case 11:
        settings->setValue("screensize","320x240");
        break;
    case 12:
        settings->setValue("screensize","240x320");
        break;
    
    }

    switch (formGeneral.comboBox->currentIndex()){
    case 0:
        settings->setValue("lang","en");
        break;
    case 1:
        settings->setValue("lang","pt_br");
        break;
    case 2:
        settings->setValue("lang","es");
        break;
    }

    settings->setValue("parameters",formRun.textEdit->toPlainText());
    settings->setValue("gingaconfig_file", formRun.lineEdit_2->text());

    saveGingaPreferences();
}

void QnplPreferencesDialog::showPreferencesItem(QModelIndex index)
{
    if (index == formPreferences.dTree->model()->index(0,0)){
        wGeneral->show();
        wRun->hide();
        formPreferences.dLabel->setText(tr("General"));

    }else{
        wGeneral->hide();
        wRun->show();
        formPreferences.dLabel->setText(tr("Run"));
    }
}

void QnplPreferencesDialog::loadGingaPreferences()
{
    if (QFile::exists(formRun.lineEdit_2->text())){

        QFile* file = new QFile(formRun.lineEdit_2->text());

        if (file->open(QIODevice::ReadOnly)){
            QMap<QString, QString> params;

            QTextStream* stream = new QTextStream(file);

            QRegExp rx; rx.setPattern("(.*)=(.*)");

            while(!stream->atEnd()){
                QString line =  stream->readLine().trimmed();

                qDebug() << line;

                if (line.contains(rx)){
                    rx.indexIn(line);

                    params[rx.cap(1).trimmed()] = rx.cap(2).trimmed();
                }
            }

            delete stream;

            delete formRun.table->model();
            formRun.table->setModel(NULL);

            int ncol = 2;
            int nrow = 32;

            formRun.table->verticalHeader()->hide();
            formRun.table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

            QStandardItemModel *model = new QStandardItemModel(nrow, ncol);

            model->setHorizontalHeaderItem(0, new QStandardItem("Name"));
            model->setHorizontalHeaderItem(1, new QStandardItem("Value"));

            int i = 0;

            foreach(QString name, params.keys()){
                if (name != "::" && name != "||"){
                    QStandardItem* nitem = new QStandardItem(name);
                    nitem->setEditable(true);

                    QStandardItem* vitem = new QStandardItem(params[name]);

                    model->setItem(i, 0, nitem);
                    model->setItem(i, 1, vitem);

                    ++i;
                }
            }

            formRun.table->setModel(model);
        }

        delete file;
    }
}

void QnplPreferencesDialog::saveGingaPreferences()
{
    if (QFile::exists(formRun.lineEdit_2->text())){

        QFile* file = new QFile(formRun.lineEdit_2->text());

        if (file->open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream stream(file);
            stream.setCodec("UTF-8");

            stream << "::" << " = " << "0" << endl;
            stream << "||" << " = " << "0" << endl;

            int n = formRun.table->model()->rowCount();

            QAbstractItemModel* model = formRun.table->model();

            for (int i=0;i<n;i++){
                QString name = model->data(model->index(i,0)).toString();
                QString value = model->data(model->index(i,1)).toString();

                if (name != ""){
                    stream << name
                           << " = "
                           << value
                           << endl;
                }
            }
        }

        delete file;
    }
}

void QnplPreferencesDialog::browseExecutable()
{
    QString f = QFileDialog::getOpenFileName(this,tr("Open"));

    if (f != ""){
        formRun.lineEdit->setText(f);
    }
}

void QnplPreferencesDialog::browseGingaSettingsFile()
{
    QString f = QFileDialog::getOpenFileName(this,tr("Open"));

    if (f != ""){
        formRun.lineEdit_2->setText(f);

        loadGingaPreferences();
    }
}
