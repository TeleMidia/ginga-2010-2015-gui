#ifndef USERPREFERENCES_H
#define USERPREFERENCES_H

#include "page.h"
#include <QMap>
#include <QPair>

#include "richmenuitem.h"

class UserPreferences : public Page
{
public:
    explicit UserPreferences(Page * parentPage, QString language = "en", QWidget *parent = 0);

    bool eventFilter(QObject *, QEvent *);

    void updateValues ();

private:
    void persistValues ();

    QMap <QString, RichMenuItem *> _variables;
    FocusableLabel *_changeValues;
};

#endif // USERPREFERENCES_H
