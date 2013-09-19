#ifndef USERPREFERENCES_H
#define USERPREFERENCES_H

#include "page.h"

class UserPreferences : public Page
{
public:
    explicit UserPreferences(Page * parentPage, QString language = "en", QWidget *parent = 0);
};

#endif // USERPREFERENCES_H
