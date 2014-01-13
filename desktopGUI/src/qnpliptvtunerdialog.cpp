#include "qnpliptvtunerdialog.h"

#include <iostream>

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


QString QnplIPTVTunerDialog::ip()
{
    return form.lineEdit->text();
}
