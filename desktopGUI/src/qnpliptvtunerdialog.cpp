#include "qnpliptvtunerdialog.h"

#include <iostream>

QnplIPTVTunerDialog::QnplIPTVTunerDialog(QWidget *parent)
  : QDialog(parent)
{
  setWindowTitle(tr("IP Address"));

  _form.setupUi(this);
  _form.lineEdit->setInputMask("000.000.000.000");
}

QnplIPTVTunerDialog::~QnplIPTVTunerDialog()
{
}

QString QnplIPTVTunerDialog::ip()
{
  return _form.lineEdit->text();
}
