#ifndef MENUITEM_H
#define MENUITEM_H

#include <QWidget>
#include <QLabel>

class MenuItem : public QWidget
{
    Q_OBJECT
public:
    explicit MenuItem(QString title = "", QString description = "", QString link = "",
                      QPair <QString, QString> _enclosure = QPair <QString, QString> (), bool isActive = true, QWidget *parent = 0);
    
    inline void setFont (const QFont &font)
    {
        _textLabel->setFont(font);
        QWidget::setFont(font);
    }

    inline bool isActive () const
    { return _isEnabled; }

    inline QString description () const
    { return _description; }

    inline QString link () const
    { return _link; }

    inline QString text () const
    { return _textLabel->text(); }

    inline QPair <QString, QString> enclosure () const
    { return _enclosure; }

    inline void setText (QString text)
    {
        _textLabel->setText(text);
        if (text == "") setFocusPolicy(Qt::NoFocus);
    }

    inline void setDescription (QString description)
    { _description = description; }

    inline void setLink (QString link)
    {_link = link; }

    inline void setEnclosure (QPair<QString,QString> enclosure)
    { this->_enclosure = enclosure; }

    inline void setEnable (bool isEnabled)
    {
        this->_isEnabled = isEnabled;
        if (_isEnabled)
            setFocusPolicy(Qt::StrongFocus);
        else
            setFocusPolicy(Qt::NoFocus);
    }

    bool eventFilter(QObject *, QEvent *);

protected:
    void paintEvent(QPaintEvent *);

signals:
    void focusIn (MenuItem *);
    void selected (MenuItem *);
    void gingaRequested (QString);

public slots:
    
private:
    QString _description;
    QString _link;
    QPair <QString, QString> _enclosure; // <url, type>
    QLabel * _textLabel;
    bool _isEnabled;
};

#endif // MENUITEM_H
