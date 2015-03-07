#ifndef QNPLIPTVTUNERDIALOG_H
#define QNPLIPTVTUNERDIALOG_H

#include "ui_qnpliptvtunerform.h"

class IptvTunerDialog : public QDialog
{
    Q_OBJECT

public:
    IptvTunerDialog(QWidget* parent = 0);
    ~IptvTunerDialog();
    QString ip();
    QString port();

public slots:
    void accept ();

private:
  Ui::QnplIPTVTunerForm _form;
};

#endif // QNPLIPTVTUNERDIALOG_H
