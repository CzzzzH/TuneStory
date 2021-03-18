#include "sqlconnect.h"
#include "ui_contral.h"
#include "btn_event.h"
#include "beatmap.h"
#include "data.h"
#include "item.h"
#include "ui_contral.h"
#include "inputbox.h"

QSqlDatabase SqlConnect::_Database = QSqlDatabase::addDatabase("QMYSQL");
UI* SqlConnect::_UI = nullptr;

bool SqlConnect::ReadHighScore() // Download the highscore in the account
{
    _Database.setDatabaseName("TSUserData");
    if(!_Database.open())
    {
        _UI->_LoadingState = "ReadFail";
        return false;
    }
    _Database.open();
    QSqlQuery _Query(_Database);
    for (qint32 t = 0; t < _UI->_SongList.size(); ++t)
    {
        auto C = _UI->_SongList[t];
        for (qint32 k = 0; k < 4; ++k)
        {
            _Query.exec("select HighScore from "  + _UI->_PlayerName +
                " where SongID = '" + C->_MapHash + QString::number(k) + "';");
            if (!_Query.isValid()) C->_HighScore[k] = "0";
            while (_Query.next())
                C->_HighScore[k] = _Query.value(0).toString();
            C->_HighScoreName[k] = _UI->_PlayerName;
            C->_HighRank[k] = Data::getrank(C->_HighScore[k].toInt());
        }
    }
    _UI->_LoadingState = "ReadSuccess";
    return true;
}

bool SqlConnect::DownloadScore()
{
    for (qint32 k = 0; k < 4 * _UI->_SongList.size(); ++k)
        for (qint32 i = 1; i <= 10; ++i)
        {
            _UI->_RankInfo[k]._Name[i] = "还没人打过哟";
            _UI->_RankInfo[k]._Score[i] = "0";
            _UI->_RankInfo[k]._Date[i] = "";
        }
    _Database.setDatabaseName("TSScoreData");
    if(!_Database.open())
    {
        _UI->_LoadingState = "DownloadFail";
        return false;
    }
    _Database.open(); // download the ranklist of the Song
    QSqlQuery _Query(_Database);
    for (qint32 t = 0; t < _UI->_SongList.size(); ++t)
    {
        auto C = _UI->_SongList[t];
        for (qint32 k = 0; k < 4; ++k)
        {
            _Query.exec("select PlayerName, Score, Time from "  + C->_MapHash + QString::number(k) + " order by Score DESC, Time ASC;");
            qint32 i = 1;
            while (_Query.next()) // Restore the Info
            {
                if (i > 10) break;
                _UI->_RankInfo[4 * t + k]._Name[i] = _Query.value(0).toString();
                _UI->_RankInfo[4 * t + k]._Score[i] = _Query.value(1).toString();
                _UI->_RankInfo[4 * t + k]._Date[i] = _Query.value(2).toString();
                if (_UI->_RankInfo[4 * t + k]._Date[i] != "") // Deal the string of date
                {
                    _UI->_RankInfo[4 * t + k]._Date[i].chop(4);
                    _UI->_RankInfo[4 * t + k]._Date[i][10] = ' ';
                }
                ++i;
            }
        }
    }
    _UI->_LoadingState = "DownloadSuccess";
    return true;
}

bool SqlConnect::UploadScore()
{
    const auto C = _UI->_SongList[_UI->_CurrentSong];
    const qint32 D = _UI->_Difficulty;
    QString _Date;
    if (QString::number(qint32(_UI->_TotalScore)) != C->_HighScore[D] || _UI->_ifBeginner) // judge if it is the highest score
    {
       _UI->_LoadingState = "NoHighScore";
        return false;
    }
    if (!_UI->_iflogin) // judge if the player has logined
    {
        _UI->_LoadingState = "NoLogin";
        return false;
    }
    for (qint32 i = 0; i < 5; ++i)
    {
        _Date = Data::getdate();
        if (_Date != "denied --" && _Date != "") break;
    }
    _Database.setDatabaseName("TSUserData");
    if (!_Database.open())
    {
        _UI->_LoadingState = "UploadFail";
        return false;
    }
    _Database.open(); // upload one part of the play's data
    QSqlQuery _Query(_Database);
    _Query.exec("delete from " + _UI->_PlayerName +
                " where SongID = '" + C->_MapHash + QString::number(D) + "';");
    _Query.exec("insert into " + _UI->_PlayerName + " values('" + C->_MapHash + QString::number(D) + "'," +
                QString::number(qint32(_UI->_TotalScore)) +
                ", '9999-01-01 00:00:00');");
    _Query.exec("update " + _UI->_PlayerName + " SET Time =" +
                " '" + _Date + "' " +
                " where SongID = '" + C->_MapHash + QString::number(D) + "');");
    _Database.close();
    _Database.setDatabaseName("TSScoreData");
    if (!_Database.open())
    {
        _UI->_LoadingState = "UploadFail";
        return false;
    }
    _Database.open(); // upload the other part of the play's data
    _Query.exec("create table if not exists " + C->_MapHash + QString::number(D) + " ("
                "ID int(4) not null auto_increment,"
                "PlayerName varchar(50) not null,"
                "Score int(4) not null,"
                "Time datetime not null,"
                "PRIMARY KEY (ID)"
                ")ENGINE=InnoDB DEFAULT CHARSET=utf8;");
    _Query.exec("delete from " + C->_MapHash + QString::number(D) +
                " where PlayerName = '" + _UI->_PlayerName + "' ;");
    _Query.exec("insert into " + C->_MapHash + QString::number(D) + " values( NULL, '" +
                _UI->_PlayerName + "', " + QString::number(qint32(_UI->_TotalScore)) + ", '9999-01-01 00:00:00');");
    _Query.exec("update " + C->_MapHash + QString::number(D) + " SET Time =" +
                " '" + _Date + "' " + " where PlayerName = '" + _UI->_PlayerName + "';");
    _UI->_LoadingState = "UploadSuccess";
    _Database.close();
    return true;
}

bool SqlConnect::Login()
{
    QString _Name = _UI->_NameBoard->text();
    QString _Password = Data::getSHA224(_UI->_PasswordBoard->text());
    _Database.setDatabaseName("TSUserData");
    if(!_Database.open())
    {
        _UI->_LoadingState = "LoginFail";
        return false;
    }
    _Database.open();
    QSqlQuery _Query(_Database);
    QString _PasswordSearch;
    _Query.exec("select PassWord from NameList where Name = '" + _Name + "';");
    while (_Query.next()) // Check if the password is right
        _PasswordSearch = _Query.value(0).toString();
    if (_PasswordSearch != _Password)
    {
        _UI->_LoadingState = "LoginWrong";
        _Database.close();
        return false;
    }
    _Query.exec("update NameList SET Time = "
                " '" + Data::getdate() + "' " + " where PlayerName = '" + _Name + "';");
    _UI->_iflogin = true;
    _UI->_PlayerName = _UI->_NameBoard->text();
    _UI->_LoadingState = "LoginSuccess";
    _Database.close();
    return true;
}

bool SqlConnect::CreateAccount()
{
    QString _Name = _UI->_NameBoard->text();
    QString _Password = Data::getSHA224(_UI->_PasswordBoard->text());
    if (_Name == "" || _UI->_PasswordBoard->text() == "")
    {
        _UI->_LoadingState = "RegisterSpace";
        return false;
    }
    _Database.setDatabaseName("TSUserData");
    if(!_Database.open())
    {
        _UI->_LoadingState = "RegisterFail";
        return false;
    }
    _Database.open();
    QSqlQuery _Query(_Database);
    QString _NameSearch;
    _Query.exec("select Name from NameList where Name = '" + _Name + "';");
    while (_Query.next()) // Check if the name exists.
        _NameSearch = _Query.value(0).toString();
    if (_NameSearch != "")
    {
        _UI->_LoadingState = "RegisterWrong";
        _Database.close();
        return false;
    }
    qint32 _UID = 0;
    _Query.exec("select max(UID) from NameList;");
    while (_Query.next()) // Create a uid for the account
        _UID = _Query.value(0).toInt() + 1;
    _Query.exec("insert into NameList(UID,Name,Password,HeadPortrait,LoginDate) "
                "values(" + QString::number(_UID) + ",'" + _Name + "','" + _Password +
                "',0,'9999-01-01 00:00:00');");
    _Query.exec("update NameList SET Time = "
                " '" + Data::getdate() + "' " + " where PlayerName = '" + _Name + "';");
    _Query.exec("create table if not exists " + _Name + " ("
                "SongID varchar(50) not null,"
                "HighScore int(4) not null,"
                "Time datetime not null,"
                "PRIMARY KEY (SongID)"
                ")ENGINE=InnoDB DEFAULT CHARSET=utf8;");
    _UI->_iflogin = true;
    _UI->_PlayerName = _UI->_NameBoard->text();
    _UI->_LoadingState = "RegisterSuccess";
    _Database.close();
    return true;
}


/*
Warning: This database address is used for demo, and it is obviously unavailable now.
*/
void SqlConnect::Initialize() // Initialize the database
{
    _Database.setHostName("39.107.114.222");
    _Database.setDatabaseName("TSUserData");
    _Database.setPort(3306);
    _Database.setUserName("TuneStory");
    _Database.setPassword("T0Vis1tTHED4T48ASe");
}

SqlConnect *SqlConnect::instance() // Get the only object of the singleton class
{
    static SqlConnect _SqlConnect;
    return &_SqlConnect;
}

SqlConnect::SqlConnect(QObject *parent)
    : QObject(parent)
{
}

SqlConnect::~SqlConnect()
{
}
