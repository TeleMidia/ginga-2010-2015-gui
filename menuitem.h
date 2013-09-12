#ifndef MENUITEM_H
#define MENUITEM_H

#include <QWidget>
#include <QLabel>

class MenuItem : public QWidget
{
    Q_OBJECT
public:
    explicit MenuItem(QString title = "", QString description = "", QString link = "",
                      QPair <QString, QString> _enclosure = QPair <QString, QString> (), QWidget *parent = 0);
    
    inline QString description () const { return _description; }
    inline QString link () const { return _link; }
    inline QString text () const { return _textLabel->text(); }
    inline QPair <QString, QString> enclosure () const { return _enclosure; }

    inline void setText (QString text) { _textLabel->setText(text); }
    inline void setDescription (QString description) { _description = description; }
    inline void setLink (QString link) {_link = link; }
    inline void setEnclosure (QPair<QString,QString> enclosure) { this->_enclosure = enclosure; }

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
};

#endif // MENUITEM_H
