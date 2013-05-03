#include "qnpliptvtunerdialog.h"

QnplIPTVTunerDialog::QnplIPTVTunerDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Insira o ip"));

    // setting
    form.setupUi(this);


}



QnplIPTVTunerDialog::~QnplIPTVTunerDialog()
{

}
