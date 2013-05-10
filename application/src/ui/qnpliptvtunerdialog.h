#ifndef QNPLIPTVTUNERDIALOG_H
#define QNPLIPTVTUNERDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QDateTime>

#include "ui_qnpliptvtunerform.h"

class QnplIPTVTunerDialog :public QDialog
{
    Q_OBJECT

public:
    QnplIPTVTunerDialog(QWidget* parent = 0);
    ~QnplIPTVTunerDialog();


private:
  Ui::QnplIPTVTunerForm form;
};

#endif // QNPLIPTVTUNERDIALOG_H