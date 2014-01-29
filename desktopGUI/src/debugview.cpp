#include "include/debugview.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

DebugView::DebugView(QWidget *parent) :
  QDockWidget("Debug View", parent)
{
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(new QLabel ("Medias running:"));

  _listWidget = new QListWidget;
  mainLayout->addWidget(_listWidget);

  QWidget *debugWidget = new QWidget (this);
  debugWidget->setLayout(mainLayout);

  setWidget(debugWidget);
}

void DebugView::addObject(QString object, int ident)
{
  for (int i = 0; i < ident; i++)
    object = " " + object;
  _listWidget->addItem(object);
}

void DebugView::removeObject(QString object, int ident)
{
  for (int i = 0; i < ident; i++)
    object = " " + object;

  QList <QListWidgetItem *> list = _listWidget->findItems(object,
                                                          Qt::MatchWrap);
  for (int i = list.count() - 1; i >= 0; i--)
    _listWidget->takeItem(_listWidget->row(list.at(i)));
}
