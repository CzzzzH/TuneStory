#include "note.h"
#include "ui_contral.h"
#include "btn_event.h"
#include "beatmap.h"
#include "data.h"
#include <QtMath>

void Note::judge(const qint32 & _Colourtype, const Note* _Dealtnote) // Judge the hit
{
    if (_Colourtype != _ColourNum) return;
    if (_Dealtnote != this) return;
    const auto C = _UI->_SongList[_UI->_CurrentSong];
    const qint32 D = _UI->_Difficulty;
    _Error = qint32(qFabs(_UI->_CurrentSongTime - 1000 * C->_NoteTime[D][_Num] - _UI->_HitOffset + _UI->_MusicOffset));
    if (_Error > 120) return;
    _UI->_NoteQueue[_ColourNum]->dequeue();
    _UI->_Combo ++;
    _UI->_MaxCombo = qMax(_UI->_Combo, _UI->_MaxCombo);
    _CurrentComboNum = _UI->_Combo;
    if (_Error <= 32 || (_UI->_ifBeginner && _Error <= 64))
    {
        _RainbowLight.setPixmap(*_UI->_NotePic[_Colourstyle + 18]);
        _UI->_RainbowHit ++;
        _UI->_NoteScore += _UI->_AvgNote;
        _RainbowLightScale = (- 0.525) * qExp((- 10) * _TimeL ) + 1.1;
        _RainbowLightAlpha = 0;
        _RainbowLight.RefreshPicture(_RainbowLightScale, _RainbowLightAlpha, x, y, _LightSize);
        _RainbowLight.show_with_raise();
        _Light.hide();
    }
    else
    {
        _UI->_NormalHit ++;
        _UI->_NoteScore += _UI->_AvgNote * 0.6;
        _RainbowLight.hide();
    }
    if (_UI->_Combo >= C->_NoteNum[D] * 0.1 && _UI->_ComboScore < 20000)
        _UI->_ComboScore += _UI->_ComboLow;
    if (_UI->_Combo >= C->_NoteNum[D] * 0.3 && _UI->_ComboScore < 20000)
        _UI->_ComboScore += (_UI->_Combo - C->_NoteNum[D] * 0.3) * _UI->_ComboHigh;
    if (_UI->_ComboScore > 20000) _UI->_ComboScore = 20000;
    _InitialLightScale = _LightScale;
    if (_UI->_RainbowHit == C->_NoteNum[D])
        _UI->_NoteScore = 980000;
    _UI->RefreshScore();
    if (_CurrentComboNum >= 2)
    {
        _CurrentCombo.changeText("x " + QString::number(_CurrentComboNum));
        _CurrentCombo.setAlpha(1);
        _CurrentCombo.show_with_raise();
    }
    _TimeL = 0;
    connect(&_UI->_NoteTimer, &QTimer::timeout, this, [=]
    {
        if (!_isJudge) return;
        if (_TimeL <= 0.3)
        {
            _TimeL += 0.016;
            if (_Error <= 32 || (_UI->_ifBeginner && _Error <= 64))
            {
                _RainbowLightScale = (- 0.525) * qExp((- 10) * _TimeL ) + 1.1;
                _RainbowLightAlpha = qExp((- 10) * _TimeL );
                _RainbowLight.RefreshPicture(_RainbowLightScale, _RainbowLightAlpha, x, y, _LightSize);
            }
            else
            {
                _LightScale = (- 0.2) * qExp((- 10) * _TimeL) + _InitialLightScale + 0.2;
                _LightAlpha = qExp((- 10) * _TimeL );
                _Light.RefreshPicture(_LightScale, _LightAlpha, x, y, _LightSize);
            }
            return;
        }
        _RainbowLight.hide();
        _Light.hide();
        _isJudge = false;
    });
}

void Note::vanish() // play the vanish animation
{
    if (!_UI->_NoteQueue[_ColourNum]->isEmpty())
        if (_UI->_NoteQueue[_ColourNum]->head() == this)
        {
            _UI->_Miss ++;
            _UI->_Combo = 0;
            _UI->_NoteQueue[_ColourNum]->dequeue();
        }
    if (_UI->_Combo && _UI->_ifHitSound) _HitSound.play();
    _TimeN = 0;
    connect(&_UI->_NoteTimer, &QTimer::timeout, this, [=]
    {
        if (_TimeN <= 0.3)
        {
            _CurrentCombo.textmove(1800, 1000 - qint32(_TimeN * 100));
            if (_UI->_Combo > _CurrentComboNum) _CurrentCombo.hide();
            else _CurrentCombo.setAlpha((- 3.3) * _TimeN + 1);
        }
        if (_TimeN <= 0.5)
        {
            _TimeN += 0.016;
            _NoteAlpha = (- 2) * _TimeN + 0.8;
            _Note.RefreshPicture(_NoteScale, _NoteAlpha, x, y, _NoteSize);
            return;
        }
        disconnect(&_UI->_NoteTimer, nullptr, this ,nullptr);
        delete this;
    });
}

void Note::play()
{
    const auto C = _UI->_SongList[_UI->_CurrentSong];
    const qint32 D = _UI->_Difficulty;
    _Note.show_with_raise();
    _Light.show_with_raise();
    _Note.RefreshPicture(_NoteScale, _NoteAlpha, x, y, _NoteSize);
    _Light.RefreshPicture(_LightScale, _LightAlpha, x, y, _LightSize);
    connect(&_UI->_NoteTimer, &QTimer::timeout, this, [=]
    {
        if (!_isPlay) return;
        _TimeN = _UI->_CurrentSongTime - 1000 * C->_NoteTime[D][_Num] + 700 + _UI->_MusicOffset;
        _TimeN /= 1000;
        if (_UI->_NoteQueue[_ColourNum]->count(this) && _TimeN <= 0.82)
        {       
            if (_TimeN <= 0.2) _NoteScale = 0.5 * _TimeN  + 0.65;
            if (_TimeN <= 0.2) _NoteAlpha = 4 * _TimeN;
            if (_TimeN <= 0.7) _LightScale = (- 0.97) * _TimeN * _TimeN + 0.75;
            if (_TimeN <= 0.7) _LightAlpha = 1.07 * _TimeN + 0.25;
            if (_TimeN > 0.7 && _TimeN <= 0.8) _LightAlpha = (- 10) * (_TimeN - 0.7) + 1;
            _Note.RefreshPicture(_NoteScale, _NoteAlpha, x, y, _NoteSize);
            _Light.RefreshPicture(_LightScale, _LightAlpha, x, y, _LightSize);
            return;
        }
        _isPlay = false;
        vanish();
    }); 
}

Note::Note(UI *__UI, QString __Colourstyle, qint32 __Num, qint32 _Colour, qint32 _x, qint32 _y)
    :QObject (), _CurrentCombo(__UI, "CurrentCombo", " ", "white", Data::font(0, 100), 1800, 1000), _Note(__UI), _Light(__UI), _RainbowLight(__UI)
{
    _UI = __UI;
    _Num = __Num;
    _ColourNum = _Colour;
    _Colourstyle =  __Colourstyle.back().toLatin1() - '1'; // Change Colourstyle to int
    _NoteScale = 0.65;
    _LightScale = 0.9;
    _NoteSize = 200;
    _LightSize = 450;
    x = _x;
    y = _y;
    _NoteAlpha = 0;
    _LightAlpha = 0;
    _Note.setPixmap(*_UI->_NotePic[_ColourNum * 3 + _Colourstyle]);
    _Light.setPixmap(*_UI->_NotePic[_ColourNum * 3 + _Colourstyle + 9]);
    connect(_UI->_Key_Listener, &BTN_EVENT::hit, this, &Note::judge);
    connect(_UI, &UI::CleanStage, this, &Note::clear);
    _isPlay = true;
    _isJudge = true;
    _HitSound.setSource(QUrl("qrc:/sound/HitSound.wav"));
    _HitSound.setVolume(0.8);
}

Note::~Note()
{
}
