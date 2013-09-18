#include "gingapage.h"

#include <QVBoxLayout>

GingaPage::GingaPage(QWidget *parent) :
    QWidget(parent)
{
    _view = new QGraphicsView;
    _infoBar = new InfoBar;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(_view);
    mainLayout->addWidget(_infoBar);

    setLayout(mainLayout);
}

void GingaPage::setBarVisible(bool visible)
{
    _infoBar->setActive(visible);
}
