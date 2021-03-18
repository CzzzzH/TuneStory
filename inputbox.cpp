#include "inputbox.h"
#include "btn_event.h"
#include "data.h"
#include "ui_contral.h"

void InputBox::keyPressEvent(QKeyEvent *event) // Override the keypressevent
{
    if (event->key() == Qt::Key_Escape)
    {
        QCoreApplication::sendEvent(_UI->_Key_Listener, _UI->_EscapeEvent);
        return;
    }
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        QCoreApplication::sendEvent(_UI->_Key_Listener, _UI->_ReturnEvent);
        return;
    }
    QLineEdit::keyPressEvent(event);
}

void InputBox::focusInEvent(QFocusEvent *event) // Override the focusinevent (to make the input valid)
{
    grabKeyboard();
    QLineEdit::focusInEvent(event);
}

InputBox::InputBox(UI *__UI, QString __Type, qint32 x, qint32 y, qint32 w, qint32 h)
{
    _UI = __UI;
    _Type = __Type;
    setParent(_UI->_Screen);
    setStyleSheet("background-color:transparent; color:white;");
    setAlignment(Qt::AlignLeft);
    setGeometry(x, y, w, h);
    setFont(Data::font(1, 45, 40));
    setMaxLength(10);
    if (__Type == "PasswordBoard") setEchoMode(QLineEdit::Password);
    hide();
}

InputBox::~InputBox()
{
}
