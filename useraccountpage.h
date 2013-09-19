#ifndef USERACCOUNTPAGE_H
#define USERACCOUNTPAGE_H

#include <QMap>
#include "page.h"
#include "richmenuitem.h"

class FocusableLabel;

class UserAccountPage : public Page
{
public:
    explicit UserAccountPage(Page * parentPage, QString language = "en", QWidget *parent = 0);

    void updateValues ();

    void keyPressEvent(QKeyEvent *);
    bool eventFilter(QObject *, QEvent *);
private:
    void persistValues ();

    QMap <QString, RichMenuItem *> _items;
    FocusableLabel *_changeValuesLabel;

    RichMenuItem *_nameItem;
    RichMenuItem *_ageItem;
    RichMenuItem *_locationItem;
    RichMenuItem *_genreItem;
};

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

#endif // USERACCOUNTPAGE_H
