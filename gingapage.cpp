#include "gingapage.h"

#include <QVBoxLayout>

GingaPage::GingaPage(QWidget *parent) :
    QWidget(parent)
{

    _view = new QGraphicsView;
    _test = new QPushButton ("TESTE");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(_view);
    mainLayout->addWidget(_test);
}
