#ifndef IPTVTUNERDIALOG_H
#define IPTVTUNERDIALOG_H

#include "ui_iptvtunerform.h"

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
  Ui::IptvTunerForm _form;
};

#endif // IPTVTUNERDIALOG_H
