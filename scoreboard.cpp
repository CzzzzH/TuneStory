#include "scoreboard.h"
#include "ui_contral.h"
#include "data.h"

void ScoreBoard::move(qint32 x, qint32 y)
{
    _TransparentBoard->move(x, y + 300 + _Num * 70);
    _RankPic->move(x + 30, y + 315 + _Num * 70);
    _RankPicE->move(x + 60, y + 315 + _Num * 70);
    _NameText->move(x + 200, y + 320 + _Num * 70);
    _ScoreText->textmove(x + 500, y + 340 + _Num * 70);
    _SongRankPic->move(x + 600, y + 300 + _Num * 70);
    _DateText->move(x + 680, y + 320 + _Num * 70);
}

ScoreBoard& ScoreBoard::operator++()
{
    show();
    return *this;
}

ScoreBoard& ScoreBoard::operator--()
{
    hide();
    return *this;
}

void ScoreBoard::show()
{
    if (_NameText->text() == _UI->_PlayerName) _TransparentBoard->show_with_raise();
    else _TransparentBoard->hide();
    _RankPic->show_with_raise();
    if (_Num == 10) _RankPicE->show_with_raise();
    _NameText->show_with_raise();
    _ScoreText->show_with_raise();
    _SongRankPic->show_with_raise();
    _DateText->show_with_raise();
}

void ScoreBoard::hide()
{
    _RankPic->hide();
    if (_Num == 10) _RankPicE->hide();
    _NameText->hide();
    _ScoreText->hide();
    _SongRankPic->hide();
    _DateText->hide();
    _TransparentBoard->hide();
}

void ScoreBoard::setInfo(QString _Name, QString _Score, QString _Date)
{
    if (_Date == "9999-01-01 00:00:00") _Date = "打歌时间是个谜呢";
    _NameText->changeText(_Name);
    _ScoreText->changeText(_Score);
    _SongRankPic->setPixmap(*_UI->_RankPic[Data::getrank(_Score.toInt())]);
    _DateText->changeText(_Date);
}

ScoreBoard::ScoreBoard(UI *__UI, QString _Name, QString _Score, QString _Date, qint32 __Num)
{
    _UI = __UI;
    _Num = __Num;
    if (_Num < 10) _RankPic = new Item(_UI, "NormalPicture", _UI->_NumPic[_Num], 30, 315 + _Num * 70, 44, 55);
    else _RankPic = new Item(_UI, "NormalPicture", _UI->_NumPic[1], 30, 315 + _Num * 70, 44, 55);
    _RankPicE = new Item(_UI, "NormalPicture", _UI->_NumPic[0], 60, 315 + _Num * 70, 44, 55);
    _NameText = new Item(_UI, "NormalText", _Name, "white", Data::font(1, 30, 30), 200, 320 + _Num * 70, -2);
    _ScoreText = new Item(_UI, "NormalText", _Score, "white", Data::font(1, 30, 30), 500, 340 + _Num * 70);
    _SongRankPic = new Item(_UI, "NormalPicture", _UI->_RankPic[Data::getrank(_Score.toInt())], 600, 300 + _Num * 70, 70, 70);
    _DateText = new Item(_UI, "NormalText", _Date, "white", Data::font(1, 30, 30), 680, 320 + _Num * 70, -2);
    _TransparentBoard = new Item(_UI);
    _TransparentBoard->setGeometry(0, 300 + _Num * 70, 980, 75);
    _TransparentBoard->setStyleSheet("background-color: #FFFFCC;");
    _TransparentBoard->setAlpha(0.5);
}

ScoreBoard::~ScoreBoard()
{

}
