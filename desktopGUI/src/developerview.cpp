#include "developerview.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QKeyEvent>

DeveloperView::DeveloperView(QWidget *parent) :
    QDockWidget("Developer View", parent, Qt::Window)
{
    _index = 0;
    _sendButton = new QPushButton ("Send Command");
    _sendButton->setEnabled(false);
    _sendButton->setDefault(true);

    _commandLine = new QLineEdit;
    _commandLine->installEventFilter(this);

    _commandsSent = new QStringList;

    _history = new QTextEdit;
    _history->setReadOnly(true);

    _console = new QTextEdit;
    _console->setReadOnly(true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel ("Command:"));

    QHBoxLayout *commandLayout = new QHBoxLayout;
    commandLayout->addWidget(_commandLine);
    commandLayout->addWidget(_sendButton);

    mainLayout->addLayout(commandLayout);
    mainLayout->addWidget(new QLabel("Commands sent:"));
    mainLayout->addWidget(_history);
    mainLayout->addWidget(new QLabel("Ginga Console"));
    mainLayout->addWidget(_console);

    QWidget *dock = new QWidget;
    dock->setLayout(mainLayout);

    setWidget(dock);

    connect (_sendButton, SIGNAL(pressed()), this, SLOT(sendButtonPressed()));
    connect (_commandLine, SIGNAL(textChanged(QString)), this, SLOT(changeSendButtonStatus()));
    connect (_commandLine, SIGNAL(returnPressed()), _sendButton, SLOT(click()));
}

void DeveloperView::sendButtonPressed()
{
    QString currentCommand = _commandLine->text();
    _commandLine->clear();

    if (currentCommand != "")
    {
        _commandsSent->append(currentCommand);
        _index = _commandsSent->count();

        emit sendCommandRequested(currentCommand);
        _history->append(currentCommand);
    }
}

void DeveloperView::changeSendButtonStatus()
{
    _sendButton->setEnabled(_commandLine->text() != "");
}

bool DeveloperView::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == _commandLine && event->type() == QEvent::KeyPress)
    {
        if (((QKeyEvent *)event)->key() == Qt::Key_Up)
        {
            if (_index > 0)
            {
                _index--;
                _commandLine->setText(_commandsSent->at(_index));
            }
            return true;
        }
        else if (((QKeyEvent *)event)->key() == Qt::Key_Down)
        {
            if (_index < _commandsSent->count() - 1)
            {
                _index++;
                _commandLine->setText(_commandsSent->at(_index));
            }
            return true;
        }
    }
    return false;
}
