#include "pagexmlparser.h"
#include <QFile>
#include <QDebug>

PageXmlParser::PageXmlParser(QString path)
{
    QFile fXml (path);
    QDomDocument doc;
    QString errorMsg;
    int errorLine, errorColumn;
    if ( ! doc.setContent( &fXml, &errorMsg, &errorLine, &errorColumn ) ) {
        qDebug () << "Line " << errorLine << ": " << errorMsg;
        _error = true;
        return;
    }

    if (!doc.isNull()){
        QDomElement rootElement = doc.documentElement();

        QDomElement channelElement = rootElement.firstChildElement();
        QDomElement childElement = channelElement.firstChildElement();

        while (!childElement.isNull()){
            QString childText = childElement.text().trimmed();

            if (childElement.tagName() == "title")
                _title = childText;

            else if (childElement.tagName() == "description")
                _description = childText;

            else if (childElement.tagName() == "language")
                _languague = childText;

            else if (childElement.tagName() == "item")
                _items.append(parseItem(childElement));

            childElement = childElement.nextSiblingElement();
        }
    }
    _error = false;
}

MenuItem * PageXmlParser::parseItem(QDomElement &itemElement)
{
    MenuItem * item = new MenuItem;

    if (!itemElement.isNull()){
        QDomElement itemChild = itemElement.firstChildElement();

        while (!itemChild.isNull()){
            QString itemText = itemChild.text().trimmed();

            if (itemChild.tagName() == "title")
                item->setText(itemText);

            else if (itemChild.tagName() == "description")
                item->setDescription(itemText);

            else if (itemChild.tagName() == "link")
                item->setLink(itemText);

            else if (itemChild.tagName() == "enclosure"){
                QString url = itemChild.attribute("url");
                QString type = itemChild.attribute("type");
                item->setEnclosure (QPair <QString, QString> (url, type));
            }

            itemChild = itemChild.nextSiblingElement();
        }
    }

    return item;
}
