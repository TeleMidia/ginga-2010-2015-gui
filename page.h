#ifndef PAGE_H
#define PAGE_H

#include <QWidget>
#include <QScrollArea>

#include "util.h"
#include "menuitem.h"
#include "gingaproxy.h"

class Page : public QWidget
{
    Q_OBJECT
public:
    explicit Page(Page * parentPage, QString title = "", QString description = "", QString language = "en",
                  QList <MenuItem *> items = QList <MenuItem *>(), QWidget *parent = 0);
    
    inline void setTitle (QString title)
    { this->_title = title; }

    inline void setDescription (QString description)
    { this->_description = description; }

    inline void setLanguage (QString language)
    { this->_language = language; }

    inline void addItem (MenuItem * item)
    { _items.append(item); }

    inline QString title () const
    { return _title; }

    inline QString description () const
    { return _description; }

    inline QString language () const
    { return _language; }

    inline QList <MenuItem *> items () const
    { return _items; }

    bool eventFilter(QObject *, QEvent *);

//    void keyPressEvent(QKeyEvent *);

protected:

signals:
    void menuItemSelected (MenuItem *);
    void parentPageRequested (Page *);
    void configurePlay ();

public slots:
    void updateDescription (MenuItem *);
    void runGinga (QString);

private:    
    QString _title;
    QString _description;
    QString _language;

    QLabel * _imageLabel;
    QLabel * _descriptionLabel;

    QList <MenuItem *> _items;

    Page * _parentPage;

    QScrollArea *_itemsScrollArea;
};

#endif // PAGE_H
