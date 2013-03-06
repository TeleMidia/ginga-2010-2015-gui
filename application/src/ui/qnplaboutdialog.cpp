#include "qnplaboutdialog.h"

QnplAboutDialog::QnplAboutDialog(QWidget* parent)
  : QDialog(parent)
{
  setWindowTitle(tr("About Ginga GUI"));

  // setting
  form.setupUi(this);
  form.lbTitle->setText("Ginga GUI v"+QString(VERSION));

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
