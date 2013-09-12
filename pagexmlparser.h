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

    QString title () const { return _title; }
    QString description () const { return _description; }
    QString languague () const { return _languague; }
    QList <MenuItem *> items() const { return _items; }
    bool hasError () const { return _error; }

private:
    MenuItem * parseItem (QDomElement &itemElement);

    bool _error;
    QString _title;
    QString _description;
    QString _languague;
    QList <MenuItem *> _items;
};

#endif // PAGEXMLPARSER_H
