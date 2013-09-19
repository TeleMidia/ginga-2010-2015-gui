#include "userpreferences.h"

UserPreferences::UserPreferences(Page * parentPage, QString language, QWidget *parent)
    : Page (parentPage, "User Preferences:", "Edit user account", language, parent)
{



    _imageLabel->setPixmap(QPixmap("/usr/local/lib/ginga/gui/files/img/usermgmt.png"));
}
