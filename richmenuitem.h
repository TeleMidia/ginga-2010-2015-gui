#ifndef RICHMENUITEM_H
#define RICHMENUITEM_H

#include <QWidget>
#include <QLabel>
#include <QFont>
#include <QPaintEvent>
#include <QPainter>

#include "util.h"

class RichMenuItem : public QWidget
{
    Q_OBJECT
public:
    inline RichMenuItem(QString label, QWidget *parent = 0) : QWidget (parent)
    {
        _label = new QLabel (QString("<font color='white'>%1</font>").arg(label));
        _defaultFont = QFont ("Tiresias", SCREEN_HEIGHT * 0.02, QFont::Bold);
        _label->setFont(_defaultFont);
        setFocusPolicy(Qt::StrongFocus);
    }

    virtual void setValue (QString value) = 0;
    virtual QString value () const = 0;

    inline void paintEvent(QPaintEvent *event)
    {
        if (hasFocus()) {
            QPainter painter(this);
            painter.save();
            QBrush brush;
            brush.setColor(Qt::blue);
            brush.setStyle(Qt::SolidPattern);
            painter.setBrush(brush);

            painter.drawRoundedRect(rect(), 15, 15);
            painter.restore();
        }

        QWidget::paintEvent(event);
    }

protected:
    QLabel *_label;
    QFont _defaultFont;

};

#endif // RICHMENUITEM_H
