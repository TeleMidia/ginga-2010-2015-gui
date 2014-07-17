#include "menuitem.h"

#include <QHBoxLayout>
#include <QPaintEvent>
#include <QStyleOptionFocusRect>
#include <QPainter>
#include <QFileInfo>
#include <QDebug>
#include "util.h"

MenuItem::MenuItem(QString text, QString description, QString link, QPair<QString, QString> enclosure, bool isActive, QWidget *parent) :
    QWidget(parent)
{
    _description = description;
    _link = link;
    _enclosure = enclosure;

    QFont labelFont ("Tiresias", SCREEN_HEIGHT * 0.02, QFont::Bold);

    _textLabel = new QLabel (text);
    _textLabel->setFont(labelFont);
    _textLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addSpacing(20);
    mainLayout->addWidget(_textLabel);

    setLayout(mainLayout);

    installEventFilter(this);

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    _isEnabled = isActive;

    if (isActive)
        setFocusPolicy(Qt::StrongFocus);
    else
        setFocusPolicy(Qt::NoFocus);
}

bool MenuItem::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
        emit focusIn(this);

    else if (event->type() == QEvent::KeyPress && _link != "")
        if( ((QKeyEvent *)event)->key() == Qt::Key_Enter || ((QKeyEvent *)event)->key() == Qt::Key_Return
                || ((QKeyEvent *)event)->key() == Qt::Key_Right){
            QFileInfo fileInfo (_link);
            if (fileInfo.suffix() == "ncl" || fileInfo.suffix() == "ts" ||
                fileInfo.fileName().startsWith("ip:"))
                emit gingaRequested (_link);

            else
                emit selected (this);
        }

    return QWidget::eventFilter(obj, event);
}

void MenuItem::paintEvent(QPaintEvent *event)
{
    if (hasFocus() ) {
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
