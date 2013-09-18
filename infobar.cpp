#include "infobar.h"

#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QEventLoop>
#include <QDebug>
#include "util.h"

InfoBar::InfoBar(QWidget *parent) :
    QWidget(parent)
{
    int w = 50;
    int h = 50;

    _stopButton = new QPushButton;
    _stopButton->setIcon(QIcon(":/icons/stop"));
    _stopButton->setIconSize(QSize(w,h));
    _stopButton->setFixedSize(w + 10, h + 10);

    _inputInfo = new QLineEdit;
    _inputInfo->setEnabled(false);
    _inputInfo->setFixedHeight(h);
    _inputInfo->setStyleSheet("background-color: white");
    _inputInfo->setFont(QFont ("Tiresias", SCREEN_HEIGHT * 0.03, QFont::Bold));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(_stopButton);
    mainLayout->addWidget(_inputInfo);

    setLayout(mainLayout);

    setGeometry(QRect(QPoint (x(), SCREEN_HEIGHT), size()));
    _isActive = false;
    setVisible(false);

    setStyleSheet("background-color: transparent;");
}

void InfoBar::setActive(bool active)
{
//    int endValue = isActive() ? pos().y() + sizeHint().height() : SCREEN_HEIGHT - sizeHint().height();

//    QPropertyAnimation* anim = new QPropertyAnimation(this, "pos", this);
//    anim->setStartValue(pos());
//    anim->setDuration(250);
//    anim->setEndValue(QPoint (pos().x(), endValue));
//    anim->start(QAbstractAnimation::DeleteWhenStopped);

//    QEventLoop loop;
//    QObject::connect(anim, SIGNAL(finished()), &loop, SLOT(quit()));
//    loop.exec();

    _isActive = active;
    setVisible(_isActive);
}
