#ifndef PAGE_H
#define PAGE_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>

#include "util.h"
#include "menuitem.h"
#include "gingaproxy.h"
#include "gingapage.h"

class FocusableLabel : public QLabel
{
public:
    explicit inline FocusableLabel (QString text, QWidget *parent = 0) : QLabel (text, parent)
    { setFocusPolicy(Qt::StrongFocus);}

    inline void paintEvent(QPaintEvent *event)
    {
        if (hasFocus()){
            QPainter painter(this);
            painter.save();
            QBrush brush;
            brush.setColor(Qt::blue);
            brush.setStyle(Qt::SolidPattern);
            painter.setBrush(brush);

            painter.drawRoundedRect(rect(), 15, 15);
            painter.restore();
        }

        QLabel::paintEvent(event);
    }
};

class Page : public QWidget
{
    Q_OBJECT
public:
    explicit Page(Page * parentPage, GingaPage *gingaPage, QString title = "", QString description = "", QString language = "en",
                  QList <MenuItem *> items = QList <MenuItem *>(), QWidget *parent = 0);
    
    explicit Page (Page * parentPage, QString title = "", QString description = "", QString language = "en", QWidget *parent = 0);

    inline void setTitle (QString title)
    { this->_title = title; }

    inline void setDescription (QString description)
    { this->_description = description; }

    inline void setLanguage (QString language)
    { this->_language = language; }

    inline QString title () const
    { return _title; }

    inline QString description () const
    { return _description; }

    inline QString language () const
    { return _language; }

    virtual inline void updateValues ()
    { }

    bool eventFilter(QObject *, QEvent *);

    void setUpItems (QList <MenuItem *> items);

    void mouseMoveEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

protected:

signals:
    void menuItemSelected (MenuItem *);
    void parentPageRequested (Page *);
    void configurePlay ();

public slots:
    void updateDescription (MenuItem *);
    void runGinga (QString);

private:
    void setupLayout (Page *parentPage, GingaPage *gingaPage, QString title, QString description, QString language);

    QVBoxLayout *_itemsLayout;
protected:
    QString _title;
    QString _description;
    QString _language;

    QLabel * _imageLabel;
    QLabel * _descriptionLabel;

    Page * _parentPage;

    QScrollArea *_itemsScrollArea;

    GingaProxy *_gingaProxy;
    GingaPage *_gingaPage;
};

#endif // PAGE_H
