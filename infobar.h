#ifndef INFOBAR_H
#define INFOBAR_H

#include <QDockWidget>
#include <QPushButton>
#include <QLineEdit>

class InfoBar : public QDockWidget
{
    Q_OBJECT
public:
    explicit InfoBar(QWidget *parent = 0);

    inline void setInputInfo (QString info)
    { _inputInfo->setText(info); }

signals:
    
public slots:

private:
    QPushButton *_stopButton;
    QLineEdit *_inputInfo;

};

#endif // INFOBAR_H
