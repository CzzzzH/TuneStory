#include "item.h"
#include "btn_event.h"
#include "ui_contral.h"

void Item::hint() // play the hint
{
    if (_UI->_LastHint != nullptr) delete _UI->_LastHint;
    _UI->_LastHint = this;
    show();
    _Timer.start(20);
    _Time = 0;
    connect(&_Timer, &QTimer::timeout, this, [&]
    {
        _Time += 0.02;
        if (_Time < 0.3) return;
        if (_Time <=0.5 && _Time >=0.3)
        {
            setAlpha((_Time - 0.3) * (-5) + 1);
            return;
        }
        setAlpha(0);
        disconnect(&_Timer, nullptr, this, nullptr);
    });
}

// Initalize the pictrue item.

Item::Item(UI *__UI, QString __Type, QPixmap *__Picture, qint32 _x, qint32 _y, qint32 _w, qint32 _h, qint32 __Flag)
{
    _UI = __UI;
    _Flag = __Flag;
    _Type = __Type;
    this->setParent(_UI->_Screen);
    _Transparent.setParent(this);
    setGraphicsEffect(&_Transparent);
    setScaledContents(true);
    setPixmap(*__Picture);
    setGeometry(_x ,_y, _w, _h);
    setAlpha(1);
    hide();
}

// Initalize the text item.

Item::Item(UI *__UI, QString __Type, QString __Content, QString _Color, QFont __Font, qint32 __Midx, qint32 __Midy, qint32 __Flag, bool _hint)
{
    _UI = __UI;
    _Type = __Type;
    _Flag = __Flag;
    _MidX = __Midx;
    _MidY = __Midy;
    _Font = __Font;
    this->setParent(_UI->_Screen);
    _Transparent.setParent(this);
    setGraphicsEffect(&_Transparent);
    setStyleSheet("color:" + _Color + ";");
    setText(__Content);
    setFont(__Font); 
    adjustSize();
    setAlpha(1);
    if (_Flag == -2) setAlignment(Qt::AlignLeft);
    else setAlignment(Qt::AlignCenter);
    if (_Flag == -2) move(_MidX, _MidY);
    else textmove(_MidX, _MidY);
    hide();
    if (_hint) hint();
}

// Initalize the movie item.

Item::Item(UI *__UI, QString __Type, QMovie *__Movie, qint32 _x, qint32 _y, qint32 _w, qint32 _h, qint32 __Flag)
{
    _UI = __UI;
    _Flag = __Flag;
    _Type = __Type;
    this->setParent(_UI->_Screen);
    setScaledContents(true);
    setMovie(__Movie);
    setGeometry(_x, _y, _w, _h);
    __Movie->start();
    hide();
}

// Initalize other items;

Item::Item(UI *__UI, qint32 __Flag)
{
    _UI = __UI;
    _Flag = __Flag;
    this->setParent(_UI->_Screen);
    _Transparent.setParent(this);
    setGraphicsEffect(&_Transparent);
    setScaledContents(true);
    hide();
}

void Item::enterEvent(QEvent *event)
{
    if (_Flag < 0) return;
    _UI->_SettingBTN[_Flag]->enterEvent(event);
}

void Item::leaveEvent(QEvent *event)
{
    if (_Flag < 0) return;
    _UI->_SettingBTN[_Flag]->leaveEvent(event);
}

void Item::mousePressEvent(QMouseEvent * event)
{
    if (_Flag < 0) return;
    _UI->_SettingBTN[_Flag]->mousePressEvent(event);
}

Item::Item()
{
}

Item::~Item()
{
}
