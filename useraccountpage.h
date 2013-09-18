#ifndef USERACCOUNTPAGE_H
#define USERACCOUNTPAGE_H

#include "page.h"

class UserAccountPage : public Page
{
public:
    explicit UserAccountPage(Page * parentPage, QString title = "", QString description = "", QString language = "en", QWidget *parent = 0);
};

#endif // USERACCOUNTPAGE_H
