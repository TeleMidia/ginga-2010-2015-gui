#ifndef USERACCOUNTPAGE_H
#define USERACCOUNTPAGE_H

#include <QMap>
#include "page.h"
#include "richmenuitem.h"


class UserAccountPage : public Page
{
public:
    explicit UserAccountPage(Page * parentPage, QString language = "en", QWidget *parent = 0);

    void updateValues ();

    void keyPressEvent(QKeyEvent *);
    bool eventFilter(QObject *, QEvent *);
private:
    void persistValues ();

    QMap <QString, RichMenuItem *> _items;
    FocusableLabel *_changeValuesLabel;

    RichMenuItem *_nameItem;
    RichMenuItem *_ageItem;
    RichMenuItem *_locationItem;
    RichMenuItem *_genreItem;
};

#endif // USERACCOUNTPAGE_H
