#ifndef COMBORICHMENUITEM_H
#define COMBORICHMENUITEM_H

#include <QComboBox>

#include "richmenuitem.h"

class ComboRichMenuItem : public RichMenuItem
{
public:
    explicit ComboRichMenuItem(QString label, QStringList values, QWidget *parent = 0);

    inline void setValue(QString value)
    {
      int index = _comboBox->findText(value);
      _comboBox->setCurrentIndex( index == -1 ? 0 : index);
    }

    inline QString value() const
    { return _comboBox->currentText(); }

    void keyPressEvent(QKeyEvent *);

private:
    QComboBox *_comboBox;
};

#endif // COMBORICHMENUITEM_H
