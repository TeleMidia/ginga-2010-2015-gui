#include "ippage.h"

#include <QLabel>
#include <QPalette>

IPPage::IPPage(Page *parentPage, QString ip, QString language, QWidget *parent)
    : Page (parentPage, "Your IP Address is:", "Press BACK for go back.", language, parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QFont labelFont ("Tiresias", SCREEN_HEIGHT * 0.02, QFont::Bold);
    _fontTemplate = tr("<font color='red'>%1</font>");

    _ipLabel = new QLabel;
    setupIp(ip);
    _ipLabel->setFont(labelFont);

    mainLayout->addSpacing(20);
    mainLayout->addWidget(_ipLabel);
    mainLayout->setAlignment(_ipLabel, Qt::AlignHCenter);

    QWidget *scrollWidget = new QWidget;
    scrollWidget->setLayout(mainLayout);

    QPalette palette = this->palette();
    palette.setColor (QPalette::Window, QColor::fromRgb(0,0,0,0));

    scrollWidget->setPalette (palette);
    scrollWidget->setBackgroundRole (QPalette::Window);

    _itemsScrollArea->setWidget(scrollWidget);

    _imageLabel->setPixmap(QPixmap("/usr/local/lib/ginga/gui/files/img/netmgmt.png"));
}
