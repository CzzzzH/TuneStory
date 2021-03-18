#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QString>
#include <QFont>
#include <QtDebug>
#include <QTextStream>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QCryptographicHash>
#include <QTcpSocket>

class UI;

class Data : public QObject // A singleton class for data processing
{
       Q_OBJECT

private:

       explicit Data(QObject *parent = nullptr);
       Data(const Data &) Q_DECL_EQ_DELETE;
       Data & operator= (Data _RightHandObj) Q_DECL_EQ_DELETE;

public:

    ~Data();
    static Data * instance();
    static qint32 difficulty(const QString & _Difficulty);
    static qint32 getrank(const qreal & _TotalScore);
    static QString convert(QChar c, qint32 flag = 0);
    static QFont font(qint32 num, qint32 size, qint32 bold = 50);
    static QChar readC(QTextStream & R);
    static QString readS(QTextStream & R);
    static QString getMD5(QFile *_File);
    static QString getSHA224(const QString & _Str);
    static void writeScore(QFile *_File, const QString & _Content);
    static void readScore(QFile *_File, QString *_Name, QString *_Score, qint32 _FilterNum = 0);
    static bool checkID(QFile *_File, QString __ID);
    static QString getdate();
};

#endif // DATA_H
