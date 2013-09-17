#include "infobar.h"

#include <QHBoxLayout>
#include "util.h"

InfoBar::InfoBar(QWidget *parent) :
    QDockWidget(parent)
{
    setAllowedAreas(Qt::BottomDockWidgetArea);

    int w = 50;
    int h = 50;

    _stopButton = new QPushButton;
    _stopButton->setIcon(QIcon(":/icons/stop"));
    _stopButton->setIconSize(QSize(w,h));
    _stopButton->setFixedSize(w + 10, h + 10);

    _inputInfo = new QLineEdit;
    _inputInfo->setEnabled(false);
    _inputInfo->setFixedHeight(h);

    _inputInfo->setFont(QFont ("Tiresias", SCREEN_HEIGHT * 0.03, QFont::Bold));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(_stopButton);
    mainLayout->addWidget(_inputInfo);

    setLayout(mainLayout);
}
