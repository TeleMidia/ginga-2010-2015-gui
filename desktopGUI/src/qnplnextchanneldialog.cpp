#include <QDialog>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QDateTime>

#include "ui_qnplaplicationform.h"

class QnplnextchannelDialog : public QDialog
{
    Q_OBJECT

  public:
    QnplnextchannelDialog(QWidget* parent = 0);
    ~QnplnextchannelDialog();

private:
  Ui::Qnplaplicationform form;
};

#endif // QNPLAPLICATIONDIALOG_H

