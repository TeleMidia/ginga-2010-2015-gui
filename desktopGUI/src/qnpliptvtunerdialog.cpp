#include "qnpliptvtunerdialog.h"

#include <iostream>

QnplIPTVTunerDialog::QnplIPTVTunerDialog(QWidget *parent)
  : QDialog(parent)
{
  setWindowTitle(tr("IP Address"));
  // setting
  _form.setupUi(this);
}

QnplIPTVTunerDialog::~QnplIPTVTunerDialog()
{
}

QString QnplIPTVTunerDialog::ip()
{
  return _form.lineEdit->text();
}
