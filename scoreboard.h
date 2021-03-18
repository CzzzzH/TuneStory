#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <QObject>
#include <QString>
#include "item.h"

class UI;

class ScoreBoard : public QObject
{
    Q_OBJECT

public:

    UI *_UI;
    Item *_NameText;
    Item *_ScoreText;
    Item *_DateText;
    Item *_SongRankPic;
    Item *_RankPic;
    Item *_RankPicE;
    Item *_ExtraRankPic;
    Item *_TransparentBoard;
    qint32 _Num;
    ScoreBoard& operator++();
    ScoreBoard& operator--();
    ScoreBoard(UI *__UI, QString _Name, QString _Score, QString _Date, qint32 __Num = -1);
    ~ScoreBoard();
    void setInfo(QString _Name, QString _Score, QString _Date);
    void show();
    void move(qint32 x, qint32 y);
    void hide();
};

#endif // SCOREBOARD_H
