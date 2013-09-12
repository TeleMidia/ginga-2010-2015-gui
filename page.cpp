#include "page.h"
#include <QVBoxLayout>

#include <QLabel>
#include <QDebug>
#include <QKeyEvent>
#include <QPropertyAnimation>
#include <QApplication>

Page::Page(unsigned long viewWID, Page *parentPage, QString title, QString description, QString language, QList<MenuItem *> items, QWidget *parent) :
    QWidget(parent)
{
    _viewWID = viewWID;

    _parentPage = parentPage;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    mainLayout->addSpacing(SCREEN_HEIGHT * 0.2);

    QFont labelFont ("Tiresias", SCREEN_HEIGHT * 0.03, QFont::Bold);

    QHBoxLayout *centerLayout = new QHBoxLayout;
    QVBoxLayout *descriptionLayout = new QVBoxLayout;
    QVBoxLayout *itemsLayout = new QVBoxLayout;

    _imageLabel = new QLabel;
    _descriptionLabel = new QLabel;

    descriptionLayout->addWidget(_imageLabel);
    descriptionLayout->addWidget(_descriptionLabel);

    _imageLabel->setFixedWidth(SCREEN_WIDTH * 0.4);

    _descriptionLabel->setFixedWidth(SCREEN_WIDTH * 0.4);
    _descriptionLabel->setWordWrap(true);

    centerLayout->addSpacing(20);
    centerLayout->addLayout(descriptionLayout);
    centerLayout->addSpacing(50);
    centerLayout->addLayout(itemsLayout);

    mainLayout->addLayout(centerLayout);

    _title = title;
    _description = description;
    _language = language;
    _items = items;

    QString fontTemplate = tr("<font color='white'>%1</font>");

    QLabel *titleLabel = new QLabel;
    titleLabel->setFont(labelFont);
    titleLabel->setText(fontTemplate.arg(title));

    labelFont.setPointSize(SCREEN_HEIGHT * 0.02);

    itemsLayout->addWidget(titleLabel);

    for (int i = 0; i < _items.size(); i++){
        MenuItem * item = items.at(i);
        item->setFont(labelFont);

        if ( item->text().trimmed() != ""){
            item->setText(fontTemplate.arg(item->text()));
            item->setFocusPolicy(Qt::StrongFocus);
        }

        itemsLayout->addWidget(item);
        connect (item, SIGNAL(focusIn(MenuItem*)), this, SLOT(updateDescription(MenuItem*)));
        connect (item, SIGNAL(selected(MenuItem*)), this, SIGNAL(menuItemSelected(MenuItem*)));
        connect (item, SIGNAL(gingaRequested(QString)), this, SLOT(runGinga(QString)));
    }

    labelFont.setPointSize(SCREEN_HEIGHT * 0.02);
    _descriptionLabel->setFont(labelFont);
    _descriptionLabel->setText(fontTemplate.arg(items.at(0)->description()));

    QString imagePath = items.at(0)->enclosure().first;
    _imageLabel->setPixmap(QPixmap(imagePath));
    _imageLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

void Page::updateDescription(MenuItem *item)
{
    if (item){
        QString imagePath = item->enclosure().first;
        QString description = item->description();

        QPixmap p(imagePath);
        if (p.isNull())
            p.load(":/backgrounds/default");

        _imageLabel->setPixmap(p);
        _descriptionLabel->setText(description);
    }
}

void Page::runGinga(QString filename)
{
    emit configurePlay();

    GingaProxy *gingaProxy = GingaProxy::getInstance(GINGA_PATH, parent());

    qDebug () << filename;

    gingaProxy->run(filename);
}

void Page::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() == Qt::Key_Down)
        focusNextChild();
    else if (keyEvent->key() == Qt::Key_Up)
        focusPreviousChild();
    else if (keyEvent->key() == Qt::Key_Left)
        emit parentPageRequested (_parentPage);

    QWidget::keyPressEvent(keyEvent);
}
