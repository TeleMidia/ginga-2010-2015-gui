#include <QDesktopServices>
#include <QUrl>
#include "aboutdialog.h"
#include "util.h"
#include "gingaproxy.h"
#include <QDateTime>

AboutDialog::AboutDialog(QString execLocation, QWidget* parent)
  : QDialog(parent)
{
  GingaProxy *gingaProxy = GingaProxy::getInstance();

  setWindowTitle(tr("About Ginga GUI"));

  // setting
  _form.setupUi(this);
  _form.lbTitle->setText("Ginga GUI v" + QString(Util::VERSION));
  _form.label_2->setText("Date: "
                         + QDateTime::currentDateTime().toString("dd/MM/yyyy."));
  _form.lbTitle_1->setText("Ginga");

  gingaProxy->setBinaryPath(execLocation);
  _form.label->setText(gingaProxy->version());

  connect(_form.btMore, SIGNAL(pressed()), SLOT(showMore()));
  connect(_form.btMore_2, SIGNAL(pressed()), SLOT(showMore()));
}

void AboutDialog::showMore()
{
  QDesktopServices::openUrl(QUrl("http://www.gingancl.org.br/en/ferramentas"));
}
