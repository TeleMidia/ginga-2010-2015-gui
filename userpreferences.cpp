#include "userpreferences.h"

#include <QStringList>
#include <QVBoxLayout>
#include <QSettings>
#include <QDebug>

#include "comborichmenuitem.h"
#include "defaultrichmenuitem.h"

UserPreferences::UserPreferences(Page * parentPage, QString language, QWidget *parent)
    : Page (parentPage, "User Preferences:", "Edit user account", language, parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QSettings settings ( ":/settings/values", QSettings::IniFormat, this);
    QStringList keys = settings.allKeys();
    keys.sort();
    for (int i = 0; i < keys.size(); i++){
        QString key = keys.at(i);
        QString value = settings.value(key).toString();

        RichMenuItem *menuItem = 0;

        QStringList options;
        if (value.startsWith("$")){
            value.remove(0, 1);

            int spaceIndex = value.indexOf(" ");
            if (spaceIndex != -1){
                QString joker = value.left(spaceIndex);
                if (joker == "INTEGER"){
                    int parenthesesIndex = value.indexOf("(");
                    if (parenthesesIndex != -1){
                        value.remove(")");
                        QString interval = value.mid(parenthesesIndex + 1);

                        spaceIndex = interval.indexOf(" ");
                        if (spaceIndex != -1){
                            int begin = interval.left(spaceIndex).toInt();
                            int end = interval.mid(spaceIndex + 1).toInt();

                            for (int i = begin; i <= end; i++)
                                options << QString::number(i);
                        }
                    }
                }
            }
        }

        if (options.size() == 0)
            options = value.split(" ", QString::SkipEmptyParts);

        key = key + ":";

        if (options.size() > 1)
            menuItem = new ComboRichMenuItem (key, options);
        else
            menuItem = new DefaultRichMenuItem (key, DefaultRichMenuItem::DEFAULT);

        mainLayout->addWidget(menuItem);
    }


    QWidget *scrollWidget = new QWidget;
    scrollWidget->setLayout(mainLayout);

    _itemsScrollArea->setWidget(scrollWidget);

    _imageLabel->setPixmap(QPixmap("/usr/local/lib/ginga/gui/files/img/usermgmt.png"));


    _imageLabel->setPixmap(QPixmap("/usr/local/lib/ginga/gui/files/img/usermgmt.png"));
}
