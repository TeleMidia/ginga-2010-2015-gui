#include "gingapage.h"

#include <QVBoxLayout>

GingaPage::GingaPage(QWidget *parent) :
    QWidget(parent)
{
    _view = new QGraphicsView;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(_view);

    setLayout(mainLayout);
}

void GingaPage::setBarVisible(bool visible)
{
    _infoBar->setActive(visible);
}
