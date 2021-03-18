#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QObject>
#include <QLabel>
#include <QWidget>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include "mainwindow.h"
#include "ui_contral.h"

class Item : public QLabel
{
    Q_OBJECT

public:

    UI *_UI;
    QString _Type;
    QMatrix _Rotate;
    qint32 _MidX,_MidY;
    qint32 _Flag;
    qreal _Alpha;
    qreal _Time;
    QGraphicsOpacityEffect _Transparent;
    QTimer _Timer;
    QFont _Font;
    Item(UI *__UI, QString __Type, QPixmap *__Picture, qint32 _x, qint32 _y, qint32 _w, qint32 _h, qint32 __Flag = -1);
    Item(UI *__UI, QString __Type, QString __Content, QString _Color, QFont __Font, qint32 _Midx, qint32 _Midy, qint32 __Flag = -1, bool _Hint = false);
    Item(UI *__UI, QString __Type, QMovie *__Movie, qint32 _x, qint32 _y, qint32 _w, qint32 _h, qint32 __Flag = -1);
    Item(UI *__UI, qint32 __Flag = -1);
    Item();
    ~Item();
    inline void Rotate(qreal _Angle)
    {
        _Rotate.reset();
        _Rotate.rotate(_Angle);
        setPixmap(_UI->_LoadingL->transformed(_Rotate, Qt::SmoothTransformation));
    }
    inline void RefreshPicture(qreal _Scale, qreal __Alpha, qint32 _x, qint32 _y, qreal _Size)
    {
        setGeometry(qint32(_x - _Size * _Scale / 2), qint32(_y - _Size * _Scale / 2), qint32(_Size  *_Scale), qint32(_Size  * _Scale));
        setAlpha(__Alpha);
    }
    inline void setAlpha(qreal __Alpha)
    {
        _Transparent.setOpacity(__Alpha);
        _Alpha = __Alpha;
    }
    inline void move(qint32 _x, qint32 _y)
    {
        setGeometry(_x, _y, width(), height());
    }
    inline void textmove(qint32 _x, qint32 _y)
    {
        move(_x - width() / 2, _y - height() / 2);
    }
    inline void show_with_raise()
    {
        show();
        raise();
    }
    inline void changeText(QString _Text, qint32 _Size = -1, QString _Color = "")
    {
        if (_Size != -1) _Font.setPixelSize(_Size);
        if (_Color != "") setStyleSheet("color:" + _Color + ";");
        setFont(_Font);
        setText(_Text);
        adjustSize();
        if (_Flag == -2) move(_MidX,_MidY);
        else textmove(_MidX, _MidY);
    }
    inline void changeSize(qint32 _Size)
    {
        _Font.setPixelSize(_Size);
        adjustSize();
        if (_Flag == -2) move(_MidX,_MidY);
        else textmove(_MidX, _MidY);
    }
    void hint();

private:

    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mousePressEvent(QMouseEvent * event);

};

#endif // ITEM_H
