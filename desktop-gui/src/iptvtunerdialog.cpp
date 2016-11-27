#include "iptvtunerdialog.h"

#include <iostream>
#include <QPushButton>
#include <QRegExpValidator>
#include <QDebug>
#include <QMessageBox>

IptvTunerDialog::IptvTunerDialog(QWidget *parent)
  : QDialog(parent)
{
  setWindowTitle(tr("IP Address"));

  _form.setupUi(this);
  _form.lineEdit->setInputMask("000.000.000.000");
}

IptvTunerDialog::~IptvTunerDialog()
{
}

QString IptvTunerDialog::ip()
{
  return _form.lineEdit->text();
}

QString IptvTunerDialog::port()
{
  return QString::number(_form._port->value());
}

void IptvTunerDialog::accept()
{
  QRegExpValidator validator;
  validator.setRegExp(
        QRegExp("(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.)"
                "{3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])"));

  QString str = _form.lineEdit->text();
  int pos = 0;

  if (validator.validate(str, pos) == QValidator::Acceptable)
    QDialog::accept();

  else
    QMessageBox::critical(this, "Error", "Invalid IP address. Please enter a "
                          "valid IP.", QMessageBox::Ok);



}
