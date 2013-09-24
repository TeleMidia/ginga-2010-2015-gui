#include "userpreferences.h"

#include <QStringList>
#include <QVBoxLayout>
#include <QSettings>
#include <QDebug>

#include "util.h"
#include "comborichmenuitem.h"
#include "defaultrichmenuitem.h"

UserPreferences::UserPreferences(Page * parentPage, QString language, QWidget *parent)
    : Page (parentPage, "User Preferences:", "Edit user account", language, parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QSettings settings ( ":/settings/values", QSettings::IniFormat, this);
    QStringList keys = settings.allKeys();
    keys.sort();
    foreach (QString key, keys){
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

        key.remove(":");
        _variables.insert(key, menuItem);

        mainLayout->addWidget(menuItem);
    }

    updateValues();

    QFont labelFont ("Tiresias", SCREEN_HEIGHT * 0.025, QFont::Bold);

    _changeValues = new FocusableLabel(QString ("<font color='white'>%1</font>").arg("> ChangeValues"));
    _changeValues->installEventFilter(this);
    _changeValues->setFont(labelFont);

    mainLayout->addSpacing(50);
    mainLayout->addWidget(_changeValues);


    QWidget *scrollWidget = new QWidget;
    scrollWidget->setLayout(mainLayout);

    _itemsScrollArea->setWidget(scrollWidget);

    _imageLabel->setPixmap(QPixmap("/usr/local/lib/ginga/gui/files/img/usermgmt.png"));
}

void UserPreferences::updateValues()
{
    QSettings gingaPreferences (USER_PREFERENCES_PATH, QSettings::IniFormat, this);
    QStringList keys = gingaPreferences.allKeys();
    foreach (QString key, keys){
        RichMenuItem *menuItem = _variables.value(key);
        if (menuItem)
            menuItem->setValue(gingaPreferences.value(key).toString());
    }
}

bool UserPreferences::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = (QKeyEvent *) event;
        if (obj == _changeValues && (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)){
            persistValues ();
            emit parentPageRequested(_parentPage);
            return true;
        }
    }

    return Page::eventFilter(obj, event);
}

void UserPreferences::persistValues()
{
    QFile preferencesFile (USER_PREFERENCES_PATH);

    if (preferencesFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        preferencesFile.write("::\t\t\t\t = 0\n");
        preferencesFile.write("||\t\t\t\t = 0\n");

        for (QMap <QString, RichMenuItem *>::iterator it = _variables.begin(); it != _variables.end(); it++){
            preferencesFile.write(QString(it.key() + "\t = " + it.value()->value() + "\n").toStdString().c_str());
        }
    }

    preferencesFile.close();
}
