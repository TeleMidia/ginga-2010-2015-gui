#ifndef QNPLIPTVTUNERDIALOG_H
#define QNPLIPTVTUNERDIALOG_H

#include "ui_qnpliptvtunerform.h"

class QnplIPTVTunerDialog : public QDialog
{
    Q_OBJECT

public:
    QnplIPTVTunerDialog(QWidget* parent = 0);
    ~QnplIPTVTunerDialog();
    QString ip();

private:
  Ui::QnplIPTVTunerForm _form;
};

#endif // QNPLIPTVTUNERDIALOG_H
