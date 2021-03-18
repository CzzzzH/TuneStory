#ifndef NOTE_H
#define NOTE_H

#include <QObject>
#include <QtDebug>
#include <QGraphicsOpacityEffect>
#include <QSoundEffect>
#include <QTimer>
#include "item.h"

class UI;

class Note : public QObject
{
    Q_OBJECT

public:

    UI *_UI;
    qint32 _Num;
    qint32 _ColourNum;
    qint32 _Colourstyle;
    qint32 _CurrentComboNum;
    qint32 x,y;
    qint32 _Error;
    qreal _TimeN;
    qreal _TimeL;
    qreal _NoteScale;
    qreal _LightScale;
    qreal _NoteSize;
    qreal _LightSize;
    qreal _NoteAlpha;
    qreal _LightAlpha;
    qreal _RainbowLightScale;
    qreal _RainbowLightAlpha;
    qreal _InitialLightScale;
    bool _isPlay;
    bool _isJudge;
    QSoundEffect _HitSound;
    Item _Note;
    Item _Light;
    Item _RainbowLight;
    Item _CurrentCombo;
    Note(UI *__UI, QString _ColourStyle, qint32 __Num, qint32 _Colour, qint32 _x, qint32 _y);
    ~Note();
    void play();
    void vanish();
    void judge(const qint32 & _Colourtype, const Note* _Dealtnote);
    inline void clear()
    {
        delete this;
    }
};

#endif // NOTE_H
