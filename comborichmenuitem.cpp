#include "comborichmenuitem.h"

#include <QHBoxLayout>
#include <QKeyEvent>

ComboRichMenuItem::ComboRichMenuItem(QString label, QStringList values, QWidget *parent)
    : RichMenuItem(label, parent)
{
    _comboBox = new QComboBox;
    _comboBox->addItems(values);
    _comboBox->setFont(_defaultFont);
    _comboBox->setStyleSheet("background-color: transparent;"
                             "color: white;"
                             "border-radius: 10px;"
                             "border: 2px solid rgb(173, 216, 230);"
                             );

    _comboBox->setFocusPolicy(Qt::NoFocus);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addSpacing(20);
    mainLayout->addWidget(_label);
    mainLayout->addWidget(_comboBox);

    setLayout(mainLayout);
}

void ComboRichMenuItem::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        _comboBox->showPopup();
    }

    RichMenuItem::keyPressEvent(event);
}
