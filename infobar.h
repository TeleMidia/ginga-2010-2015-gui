#ifndef INFOBAR_H
#define INFOBAR_H

#include <QDockWidget>
#include <QPushButton>
#include <QLineEdit>

class InfoBar : public QWidget
{
    Q_OBJECT
public:
    explicit InfoBar(QWidget *parent = 0);

    void setActive(bool active);

    inline void setInputInfo (QString info)
    { _inputInfo->setText(info); }

    inline bool isActive () const
    { return _isActive; }

    inline QSize sizeHint() const
    { return QSize(width(), 50); }

signals:
    
public slots:

private:
    QPushButton *_stopButton;
    QLineEdit *_inputInfo;
    bool _isActive;
    int _initialY;
};

#endif // INFOBAR_H
