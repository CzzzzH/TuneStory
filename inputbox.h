#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <QObject>
#include <QLineEdit>

class UI;

class InputBox : public QLineEdit
{
    Q_OBJECT

public:
    UI *_UI;
    QString _Type;
    InputBox(UI *__UI, QString __Type, qint32 x, qint32 y, qint32 w, qint32);
    ~InputBox();
    inline void show_with_raise()
    {
        show();
        raise();
    }
    void focusInEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // INPUTBOX_H
