#include "qnplaboutdialog.h"
#include "qnplutil.h"
#include "qnplsettings.h"

QnplAboutDialog::QnplAboutDialog(QWidget* parent)
  : QDialog(parent)
{
  setWindowTitle(tr("About Ginga GUI"));

  // setting
  form.setupUi(this);
  form.lbTitle->setText("Ginga GUI v" + QString(QnplUtil::VERSION));
  form.label_2->setText("Date: "+QDateTime::currentDateTime().toString("dd/MM/yyyy."));
  form.lbTitle_1->setText("Ginga");
  process = new QProcess (this);

  QStringList plist;
  plist << "--version";

  connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(printGingaVersion()));
  connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(printFailToStart(QProcess::ProcessError)));

  QnplSettings *settings = new QnplSettings();

  process->start(settings->value("location").toString(), plist);

  // connecting
  connect(form.btMore, SIGNAL(pressed()), SLOT(showMore()));
  connect(form.btMore_2, SIGNAL(pressed()), SLOT(showMore()));
}

void QnplAboutDialog::printFailToStart(QProcess::ProcessError error)
{
    if (error == QProcess::FailedToStart){
        form.label->setStyleSheet("color: red");
        form.label->setText("Cannot identify version! Binary not found.");
    }
}

void QnplAboutDialog::printGingaVersion()
{
    form.label->setText(process->readAllStandardOutput());
    process->close();
}

QnplAboutDialog::~QnplAboutDialog()
{

}

void QnplAboutDialog::showMore()
{
  QDesktopServices::openUrl(QUrl("http://www.telemidia.puc-rio.br/~edcaraujo/gingagui/"));
}
