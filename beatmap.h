#ifndef BEATMAP_H
#define BEATMAP_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QPixmap>

class UI;

class Beatmap : public QObject
{
    Q_OBJECT

public:

    qint32 _Length;
    qint32 _BPM;
    qint32 _NoteNum[4];
    qint32 _Level[4];
    qint32 _HighRank[4];
    qint32 *x[4], *y[4], *_Constant[4], *_Colour[4];
    qreal *_NoteTime[4];
    QVector<QString> _NoteStyle[4];
    QString _Path;
    QString _Name;
    QString _MusicName;
    QString _Author;
    QString _Mapper;
    QString _Difficulty;
    QString _MapHash;
    QString _HighScore[4];
    QString _HighScoreName[4];
    QPixmap _BackGroundPic;
    QPixmap _SmallPic;
    Beatmap();
    ~Beatmap();
};

#endif // BEATMAP_H
