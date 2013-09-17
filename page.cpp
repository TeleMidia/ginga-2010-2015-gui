#include "page.h"
#include <QVBoxLayout>

#include <QStyle>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QKeyEvent>
#include <QPropertyAnimation>
#include <QApplication>
#include <QGraphicsOpacityEffect>

#include "mainwindow.h"

Page::Page(Page *parentPage, GingaPage *gingaPage, QString title, QString description, QString language, QList<MenuItem *> items, QWidget *parent) :
    QWidget(parent)
{
    _gingaPage = gingaPage;
    _parentPage = parentPage;

    _gingaProxy = GingaProxy::getInstance(GINGA_PATH);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    mainLayout->addSpacing(SCREEN_HEIGHT * 0.2);

    QFont labelFont ("Tiresias", SCREEN_HEIGHT * 0.03, QFont::Bold);

    QHBoxLayout *centerLayout = new QHBoxLayout;
    QVBoxLayout *descriptionLayout = new QVBoxLayout;
    QVBoxLayout *itemsLayout = new QVBoxLayout;
    QVBoxLayout *labelsLayout = new QVBoxLayout;

    _itemsScrollArea = new QScrollArea;
    _itemsScrollArea->setFocusPolicy(Qt::NoFocus);
    _itemsScrollArea->installEventFilter(this);
    _itemsScrollArea->setFrameShape(QFrame::NoFrame);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor::fromRgb(0,0,0,0));
    _itemsScrollArea->setPalette(palette);
    _itemsScrollArea->setBackgroundRole(QPalette::Window);

    _imageLabel = new QLabel;
    _descriptionLabel = new QLabel;

    descriptionLayout->addWidget(_imageLabel);
    descriptionLayout->addWidget(_descriptionLabel);

    _imageLabel->setFixedWidth(SCREEN_WIDTH * 0.4);

    _descriptionLabel->setFixedWidth(SCREEN_WIDTH * 0.4);
    _descriptionLabel->setWordWrap(true);

    mainLayout->addLayout(centerLayout);

    _title = title;
    _description = description;
    _language = language;
    _items = items;

    QString fontTemplate = tr("<font color='white'>%1</font>");

    QLabel *titleLabel = new QLabel;
    titleLabel->setFont(labelFont);
    titleLabel->setText(fontTemplate.arg(title));

    labelsLayout->addWidget(titleLabel);
    labelsLayout->addWidget(_itemsScrollArea);

    centerLayout->addSpacing(20);
    centerLayout->addLayout(descriptionLayout);
    centerLayout->addSpacing(50);
    centerLayout->addLayout(labelsLayout);

    labelFont.setPointSize(SCREEN_HEIGHT * 0.02);

    for (int i = 0; i < _items.size(); i++){
        MenuItem * item = items.at(i);
        item->setFont(labelFont);
        item->setFocus();

        if ( item->text().trimmed() != ""){
            item->setText(fontTemplate.arg(item->text()));
            item->setFocusPolicy(Qt::StrongFocus);
        }

        itemsLayout->addWidget(item);
        connect (item, SIGNAL(focusIn(MenuItem*)), this, SLOT(updateDescription(MenuItem*)));
        connect (item, SIGNAL(selected(MenuItem*)), this, SIGNAL(menuItemSelected(MenuItem*)));
        connect (item, SIGNAL(gingaRequested(QString)), this, SLOT(runGinga(QString)));
    }

    QWidget *itemsWidget = new QWidget;
    itemsWidget->setLayout(itemsLayout);
    itemsWidget->setPalette(palette);
    itemsWidget->setBackgroundRole(QPalette::Window);
    _itemsScrollArea->setWidget(itemsWidget);

    labelFont.setPointSize(SCREEN_HEIGHT * 0.02);
    _descriptionLabel->setFont(labelFont);
    _descriptionLabel->setText(fontTemplate.arg(items.at(0)->description()));

    QString imagePath = items.at(0)->enclosure().first;
    _imageLabel->setPixmap(QPixmap(imagePath));
    _imageLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    setMouseTracking(true);
}


void Page::updateDescription(MenuItem *item)
{
    if (item){
        QString imagePath = item->enclosure().first;
        QString description = item->description();

        QPixmap p(imagePath);

        if (p.isNull())
            p.load(":/backgrounds/default");

        const QPixmap *currentPixmap = _imageLabel->pixmap();

        if (currentPixmap->pixmapData() != p.pixmapData()){

            QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(this);
            opacityEffect->setOpacity(1.0);
            _imageLabel->setGraphicsEffect(opacityEffect);

            QPropertyAnimation* anim = new QPropertyAnimation(this);
            anim->setTargetObject(opacityEffect);
            anim->setPropertyName("opacity");
            anim->setDuration(250);
            anim->setStartValue(opacityEffect->opacity());
            anim->setEndValue(0);
            anim->setEasingCurve(QEasingCurve::OutQuad);
            anim->start(QAbstractAnimation::DeleteWhenStopped);

            QEventLoop loop;
            QObject::connect(anim, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();

            _imageLabel->setGraphicsEffect(0);

            _imageLabel->setPixmap(p);
        }

        _descriptionLabel->setText(QString("<font color='white'>%1</font>").arg(description));
    }
}

void Page::runGinga(QString filename)
{
    WId wid = _gingaPage->viewWId();
//    _gingaPage->setInputInfo(filename);
    _gingaProxy->run(filename, wid);
}

bool Page::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress){
        if (obj == _itemsScrollArea ){
            QKeyEvent *keyEvent = static_cast <QKeyEvent*>(event);

            if (keyEvent->key() == Qt::Key_Up){
                focusPreviousChild();
            }
            else if (keyEvent->key() == Qt::Key_Down){
                focusNextChild();
            }
            else if (keyEvent->key() == Qt::Key_Left)
                emit parentPageRequested (_parentPage);

            _itemsScrollArea->ensureWidgetVisible(focusWidget());

            event->accept();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void Page::mouseMoveEvent(QMouseEvent *event)
{
    if (_gingaProxy->state() == QProcess::Running){
        qDebug() << "running";
    }

    QWidget::mouseMoveEvent(event);
}
