#include "qnplaboutdialog.h"

QnplAboutDialog::QnplAboutDialog(QWidget* parent)
  : QDialog(parent)
{
  setWindowTitle(tr("About Ginga GUI"));

  // setting
  form.setupUi(this);
  form.lbTitle->setText("Ginga GUI v"+QString(VERSION));
  form.label_2->setText("Date: "+QDateTime::currentDateTime().toString("dd/MM/yyyy."));
  form.lbTitle_1->setText("Ginga");
  form.label->setStyleSheet("color: red");
  form.label->setText("Cannot identify version! Binary not selected!");



  // connecting
  connect(form.btMore, SIGNAL(clicked()), SLOT(showMore()));
}

QnplAboutDialog::~QnplAboutDialog()
{

}

void QnplAboutDialog::showMore()
{
  QDesktopServices::openUrl(QUrl("http://www.telemidia.puc-rio.br/~edcaraujo/gingagui/"));
}
