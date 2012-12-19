#include "qnplaboutdialog.h"

QnplAboutDialog::QnplAboutDialog(QWidget* parent)
    : QDialog(parent)
{
    form.setupUi(this);

    setWindowTitle(tr("About Ginga"));

    setFixedSize(421,217);

    connect(form.pushButton, SIGNAL(clicked()), SLOT(showMore()));

}

QnplAboutDialog::~QnplAboutDialog()
{

}

void QnplAboutDialog::showMore()
{
    QDesktopServices::openUrl(QUrl("http://svn.softwarepublico.gov.br/trac/ginga/wiki/GingaNCL_CPP_TODO"));

    /*
    QTextEdit* text = new QTextEdit();
    text->setMinimumSize(300, 500);
    text->setStyleSheet("font-size: 12px;");
    text->setReadOnly(true);

    if (QFile::exists(QApplication::applicationDirPath()+"/about.txt")){
        QFile* file = new QFile(QApplication::applicationDirPath()+"/about.txt");

        if (file->open(QIODevice::ReadOnly)){
            QTextStream* stream = new QTextStream(file);

            QString s = stream->readAll(); text->setText(s);

            delete stream;
        }

        delete file;
    }

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(text);


    QDialog* d = new QDialog();
    d->setWindowTitle(tr("More About Ginga"));
    d->setLayout(layout);
    d->exec();

    delete d;
    */
}
