#include "defaultrichmenuitem.h"

#include <QLabel>
#include <QPaintEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QKeyEvent>

#include "util.h"

DefaultRichMenuItem::DefaultRichMenuItem(QString label, ItemType type, QWidget *parent) :
    RichMenuItem(label, parent)
{
    _type = type;
    _label->setFont(_defaultFont);

    _lineEdit = new QLineEdit;
    _lineEdit->setFocusPolicy(Qt::NoFocus);
    _lineEdit->setFont(_defaultFont);
    _lineEdit->setStyleSheet("QLineEdit {"
                             "background : transparent;"
                             "color: white;"
                             "border-radius: 10px;"
                             "border: 2px solid rgb(173, 216, 230);}");

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addSpacing(20);
    mainLayout->addWidget(_label);
    mainLayout->addWidget(_lineEdit);

    setLayout(mainLayout);

    setFocusPolicy(Qt::StrongFocus);

    installEventFilter(this);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

void DefaultRichMenuItem::keyPressEvent(QKeyEvent *event)
{
    event->accept();
    QString key = event->text();
    QString currentText = _lineEdit->text();
    int number;

    if (event->key() == Qt::Key_Up){
        focusPreviousChild();
        return;
    }
    if (event->key() == Qt::Key_Down){
        focusNextChild();
        return;
    }
    if (event->key() == Qt::Key_Backspace){
        QWidget::keyPressEvent(event);
        return;
    }

    if (event->key() == Qt::Key_Left && currentText.size()){
        currentText = currentText.remove(currentText.size() - 1, 1);
        _lineEdit->setText(currentText);

        return;
    }

    else{
        switch (_type){
        case NUMBER:
        case LOCATION:
            bool ok;
            number = QString (_lineEdit->text() + key).toInt(&ok);
            if (ok)
                _lineEdit->setText(QString::number(number));
            break;

        default:
            _lineEdit->setText(currentText + key);
        }
    }
}
