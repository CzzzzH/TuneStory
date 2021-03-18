#ifndef BTN_EVENT_H
#define BTN_EVENT_H

#include <QObject>
#include <QTimer>
#include <QKeyEvent>
#include <QApplication>
#include <QGraphicsOpacityEffect>
#include <QCoreApplication>
#include "windows.h"
#include "item.h"

class UI;
class Note;

class BTN_EVENT : public Item
{
    Q_OBJECT

public:

    bool _Available;
    BTN_EVENT(UI *__UI);
    BTN_EVENT(UI *__UI, QString __Type, QPixmap *__Picture, qint32 _x, qint32 _y, qint32 _w, qint32 _h, qint32 __Flag = -1);
    ~BTN_EVENT();
    void Hint(QString _HintText, qint32 _Fontnum, qint32 _Num);
    void setAvailable(bool _Signal);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);

signals:
    void hit(const qint32 & _Colourtype, const Note* _Dealtnote);

};

#endif // BTN_EVENT_H
