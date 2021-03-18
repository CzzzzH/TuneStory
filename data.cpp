#include "data.h"
#include "ui_contral.h"
#include "sqlconnect.h"

qint32 Data::difficulty(const QString & _Difficulty) // get the difficulty num
{
    if (_Difficulty == "Easy") return 0;
    if (_Difficulty == "Normal") return 1;
    if (_Difficulty == "Hard") return 2;
    if (_Difficulty == "Extreme") return 3;
    return 0;
}

qint32 Data::getrank(const qreal & _TotalScore) // get the rank of the score
{
    if (_TotalScore >= 980000) return 0;
    else if (_TotalScore >= 800000) return 4 - qint32(_TotalScore - 800000) / 50000;
    else if (_TotalScore >= 600000) return 5;
    else return 6;
}

QChar Data::readC(QTextStream & R) // read a char
{
    QChar C;
    R >> C;
    while (C == '\xd' || C == '\xa') R >> C;
    return C;
}

QString Data::readS(QTextStream & R) // read a line of string
{
    QString S;
    S = R.readLine();
    while (S == '\xd' || S == '\xa' || S == "") S = R.readLine();
    return S;
}

QString Data::getdate()
{
    QTcpSocket *_Socket = new QTcpSocket;
    QString _Data;
    _Socket->connectToHost("time.nist.gov", 13);
    if (_Socket->waitForConnected(1000))
    {
        if (_Socket->waitForReadyRead())
        {
            QString _Time(_Socket->readAll());
            _Time = _Time.trimmed();
            _Data = _Time.section(" ", 1, 2);
        }
    }
    _Socket->close();
    delete _Socket;
    return _Data;
}

QString Data::convert(QChar c, qint32 flag) // get the text of the key
{
    if (flag)
    {
        if (c == 'T') return "开";
        if (c == 'F') return "关";
    }
    if (c == Qt::Key_Space) return "Spc";
    if (c == Qt::Key_Tab) return "Tab";
    if (c == Qt::Key_CapsLock) return "Cap";
    if (c == Qt::Key_Backspace) return "Bak";
    if (c == Qt::Key_Delete) return "Del";
    if (c == Qt::Key_Insert) return "Ins";
    if (c == Qt::Key_Home) return "Hom";
    if (c == Qt::Key_NumLock) return "Num";
    if (c == Qt::Key_PageUp) return "Pup";
    if (c == Qt::Key_PageDown) return "Pdn";
    if (c == Qt::Key_Up) return "↑";
    if (c == Qt::Key_Down) return "↓";
    if (c == Qt::Key_Left) return "←";
    if (c == Qt::Key_Right) return "→";
    return c;
}

QFont Data::font(qint32 num, qint32 size, qint32 bold) // get the font
{
    QFont _Font;
    QString _FontType;
    if (num == 0) _FontType = "FuturaHandwritten";
    if (num == 1) _FontType = "Adobe Heiti Std R";
    if (num == 2) _FontType = "Source Han Sans CN Medium";
    _Font.setBold(true);
    _Font.setFamily(_FontType);
    _Font.setPixelSize(size);
    _Font.setWeight(bold);
    _Font.setStyleStrategy(QFont::PreferAntialias);
    return _Font;
}

QString Data::getMD5(QFile *_File) // encryption
{
    _File->close();
    _File->open(QFile::ReadOnly);
    QDataStream _DataRead(_File);
    QByteArray _ByteData;
    _ByteData = _File->readAll();
    QByteArray _ByteDataMD5 = QCryptographicHash::hash(_ByteData, QCryptographicHash::Md5);
    _File->close();
    return _ByteDataMD5.toHex();
}

QString Data::getSHA224(const QString & _Str) // encrypt users' info
{
    QByteArray _ByteDataSHA224 = QCryptographicHash::hash(_Str.toLatin1(), QCryptographicHash::Md5);
    return _ByteDataSHA224.toHex();
}

bool Data::checkID(QFile *_File, QString __ID) // anti-cheat work
{
    _File->open(QFile::ReadOnly);
    QDataStream _DataRead(_File);
    QByteArray _DataID, _ID(__ID.toUtf8());
    _DataRead >> _DataID;
    _File->close();
    if (QByteArray::fromBase64(_DataID) == _ID) return true;
    else return false;
}

void Data::writeScore(QFile *_File, const QString & _Content)
{
    _File->open(QFile::Append);
    QDataStream _DataWrite(_File);
    QByteArray _Data(_Content.toUtf8());
    _DataWrite << _Data.toBase64();
    _File->close();
    return;
}

void Data::readScore(QFile *_File, QString *_Name, QString *_Score, qint32 _FilterNum)
{
    QByteArray _Data1, _Data2, _Filter;
    QDataStream _DataRead(_File);
    for (qint32 k = 0; k < _FilterNum; ++k)
        _DataRead >> _Filter;
    _DataRead >> _Data1 >> _Data2;
    *_Name = QByteArray::fromBase64(_Data1);
    *_Score = QByteArray::fromBase64(_Data2);
    return;
}

Data *Data::instance() // Get the only object of the singleton class
{
    static Data _Data;
    return &_Data;
}

Data::Data(QObject *parent)
    : QObject(parent)
{
}

Data::~Data()
{
}
