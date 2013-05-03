#include "qnplchannelsdialog.h"

QnplChannelsDialog::QnplChannelsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Listas de canais"));

    // setting
    form.setupUi(this);




    // connecting
    //connect(form.btMore, SIGNAL(clicked()), SLOT(showMore()));
  }

QnplChannelsDialog::~QnplChannelsDialog()
{

}




