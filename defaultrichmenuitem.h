#ifndef USERMANAGEMENTMENUITEM_H
#define USERMANAGEMENTMENUITEM_H

#include <QWidget>
#include <QLineEdit>

#include "richmenuitem.h"

class DefaultRichMenuItem : public RichMenuItem
{
    Q_OBJECT
public:
    enum ItemType {
        NAME,
        AGE,
        LOCATION,
        GENRE
    };

    explicit DefaultRichMenuItem(QString label, ItemType type, QWidget *parent = 0);
    
    inline void setValue (QString value)
    { _lineEdit->setText(value);}

    inline QString value () const
    { return _lineEdit->text(); }

    void keyPressEvent(QKeyEvent *);

signals:
    
public slots:
    
private:
    QLineEdit *_lineEdit;
    ItemType _type;
};

#endif // USERMANAGEMENTMENUITEM_H
