#ifndef IPPAGE_H
#define IPPAGE_H

#include "page.h"

class IPPage : public Page
{
public:
    explicit IPPage(Page * parentPage, QString ip, QString language = "en", QWidget *parent = 0);

    inline void setupIp (QString ip)
    { _ipLabel->setText(_fontTemplate.arg( ip == "" ? "DHCP client error" : ip)); }

private:

    QString _fontTemplate;
    QLabel *_ipLabel;
};

#endif // IPPAGE_H
