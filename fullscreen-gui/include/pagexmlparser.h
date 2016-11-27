#ifndef PAGEXMLPARSER_H
#define PAGEXMLPARSER_H

#include <QDomDocument>
#include <QList>

#include "util.h"
#include "menuitem.h"

class PageXmlParser
{
public:
    explicit PageXmlParser(QString path);

    QString title () const
    { return _title != "" ? _title :
                            "NCL applications in this USB Drive:"; }
    QString description () const { return _description; }
    QString language () const { return _language; }
    QList <MenuItem *> items() const { return _items; }
    bool hasError () const { return _error; }

private:
    MenuItem * parseItem (QDomElement &itemElement);

    bool _error;
    QString _title;
    QString _description;
    QString _language;
    QList <MenuItem *> _items;
};

#endif // PAGEXMLPARSER_H
