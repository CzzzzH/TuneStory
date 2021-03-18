#include "ui_contral.h"
#include "btn_event.h"
#include "beatmap.h"
#include "note.h"
#include "item.h"
#include "inputbox.h"
#include "data.h"
#include "sqlconnect.h"
#include "scoreboard.h"

void UI::Loading(QString _Command)
{
    _LoadingState = _Command;
    if (_Command == "Connecting")
    {
        _ConnectingScreen->_Time = 0;
        _ConnectingScreen->_Timer.start(16);
        connect(&_ConnectingScreen->_Timer, &QTimer::timeout, this, [=]
        {
            if (_LoadingState == "Connecting")
            {
                _ConnectingScreen->_Time += 4;
                _ConnectingScreen->show_with_raise();
                _LoadingSign->show_with_raise();
                _LoadingSign->Rotate( _ConnectingScreen->_Time);
                return;
            }
            _ConnectingScreen->hide();
            _LoadingSign->hide();
            _ConnectingScreen->_Timer.stop();
            disconnect(&_ConnectingScreen->_Timer, nullptr, this ,nullptr);
            if (_LoadingState == "DownloadSuccess")
            {
                new Item(this, "HintText", "读取歌曲排行榜成功！"
                    , "white", Data::font(1, 40, 20), 1500, 35, -1, true);
                if (_iflogin)
                {
                    QtConcurrent::run(SqlConnect::ReadHighScore);
                    Loading("Connecting");
                }
                return;
            }
            if (_LoadingState == "DownloadFail")
            {
                new Item(this, "HintText", "读取歌曲排行榜失败！请检查网络连接"
                    , "white", Data::font(1, 40, 20), 1400, 35, -1, true);
                return;
            }
            if (_LoadingState == "ReadSuccess")
            {
                new Item(this, "HintText", "同步账号成绩成功！"
                    , "white", Data::font(1, 40, 20), 1500, 35, -1, true);
                return;
            }
            if (_LoadingState == "ReadFail")
            {
                new Item(this, "HintText", "同步账号成绩失败！请检查网络连接"
                    , "white", Data::font(1, 40, 20), 1400, 35, -1, true);
                return;
            }
            if (_LoadingState == "NoLogin")
            {
                new Item(this, "HintText", "由于你没有登录，此次最佳成绩将不会被上传orz"
                    , "white", Data::font(1, 40, 20), 1300, 35, -1, true);
                return;
            }
            if (_LoadingState == "UploadSuccess")
            {
                new Item(this, "HintText", "上传新纪录成功！"
                    , "white", Data::font(1, 40, 20), 1500, 35, -1, true);
                return;
            }
            if (_LoadingState == "UploadFail")
            {
                new Item(this, "HintText", "上传新纪录失败！请检查网络连接（按R键重试）"
                    , "white", Data::font(1, 40, 20), 1300, 35, -1, true);
                return;
            }
            if (_LoadingState == "LoginFail")
            {
                new Item(this, "HintText", "登录失败！请检查网络连接"
                    , "white", Data::font(1, 40, 20), 1500, 35, -1, true);
                return;
            }
            if (_LoadingState == "LoginWrong")
            {
                new Item(this, "HintText", "用户名或密码不正确QWQ..是不是不小心输错了呢"
                    , "white", Data::font(1, 40, 20), 1400, 35, -1, true);
                return;
            }
            if (_LoadingState == "LoginSuccess")
            {
                new Item(this, "HintText", "登录成功！", "white", Data::font(1, 40, 20), 1500, 35, -1, true);
                QCoreApplication::sendEvent(_Key_Listener, _EscapeEvent);
                Loading("Connecting");
                QtConcurrent::run(SqlConnect::ReadHighScore);
                return;
            }
            if (_LoadingState == "RegisterFail")
            {
                new Item(this, "HintText", "创建账号失败！请检查网络连接"
                    , "white", Data::font(1, 40, 20), 1500, 35, -1, true);
                return;
            }
            if (_LoadingState == "RegisterWrong")
            {
                new Item(this, "HintText", "名字已经被用过了哟> <~ 换一个叭！"
                   , "white", Data::font(1, 40, 20), 1500, 35, -1, true);
                return;
            }
            if (_LoadingState == "RegisterSpace")
            {
                new Item(this, "HintText", "用户名或密码不能为空的呢~"
                   , "white", Data::font(1, 40, 20), 1500, 35, -1, true);
                return;
            }
            if (_LoadingState == "RegisterSuccess")
            {
                new Item(this, "HintText", "创建账号成功！", "white", Data::font(1, 40, 20), 1500, 35, -1, true);
                QCoreApplication::sendEvent(_Key_Listener, _EscapeEvent);
                Loading("Connecting");
                QtConcurrent::run(SqlConnect::ReadHighScore);
                return;
            }
        });
    }
}

void UI::RefreshScore() // Refresh the ScoreTextboard in the play
{
    _TotalScore = _NoteScore + _ComboScore;
    const QString S = QString::number(qint32(_TotalScore));
    const qint32 N = S.size();
    if (_CurrentScene == "PlaySong")
        for (qint32 k = 0; k < N; ++k)
        {
            _ScoreTextBoard[k]->setGeometry(900 + 45 * (N - 2 * k - 1), 40, 120, 150);
            _ScoreTextBoard[k]->setPixmap(*_NumPic[S[N- k - 1].toLatin1() - '0']);
            _ScoreTextBoard[k]->show_with_raise();
        }
    if (_CurrentScene == "Result")
        for (qint32 k = 0; k < N; ++k)
        {
            _ScoreTextBoard[k]->setGeometry(900 + 65 * (N - 2 * k - 1), 60, 180, 225);
            _ScoreTextBoard[k]->setPixmap(*_NumPic[10 + S[N - k - 1].toLatin1() - '0']);
            _ScoreTextBoard[k]->show_with_raise();
        }
}

void UI::RefreshAccountState()
{
    for (auto it = _FunctionBTN.begin(); it != _FunctionBTN.end(); ++it) // Load the function buttons;
    {
        (*it)->hide();
        if (!_iflogin)
            if ((*it)->_Type == "Login" || (*it)->_Type == "Register") (*it)->show_with_raise();
        if (_iflogin)
            if ((*it)->_Type == "Logout") (*it)->show_with_raise();
    }
    for (auto it = _PlayerInfo.begin(); it != _PlayerInfo.end(); ++it)
    {
        if ((*it)->_Type == "PlayerName") (*it)->changeText(_PlayerName);
        if (_iflogin) (*it)->show_with_raise();
        else (*it)->hide();
    }
}

void UI::RefreshRankList(QString _Difficulty)
{
    const auto C = _SongList[_CurrentSong];
    const auto D = Data::difficulty(_Difficulty);
    _HighScoreName->changeText(C->_HighScoreName[D]);
    _HighScore->changeText(C->_HighScore[D]);
    _HighScoreRank->setPixmap(*_RankPic[C->_HighRank[D]]);
    for (qint32 k = 1; k <= 10; ++k)
        _RankList[k]->setInfo(_RankInfo[_CurrentSong * 4 + D]._Name[k], _RankInfo[_CurrentSong * 4 + D]._Score[k],_RankInfo[_CurrentSong * 4 + D]._Date[k]);
}

void UI::Result(QString _Command) // Show the result of the play
{
    const auto C = _SongList[_CurrentSong];
    const qint32 D = _Difficulty;
    if (_Command == "Show")
    {
        _CurrentScene = "Result";
        RefreshScore();
        _BackGround->setAlpha(0.3);
        _BGM->play();       
        _Rank = Data::getrank(_TotalScore);
        if (_ifBeginner)
        {
            new Item(this, "HintText", "由于你开启了新手模式，此次成绩将不会被记录orz"
                , "white", Data::font(1, 40, 20), 1300, 35, -1, true);
        }
        if (!_ifBeginner && _TotalScore > C->_HighScore[D].toInt())
        {
            C->_HighScore[D] = QString::number(qint32(_TotalScore));
            C->_HighScoreName[D] = _PlayerName;
            C->_HighRank[D] = _Rank;
        }
        for (auto it = _PlayBoard.begin(); it != _PlayBoard.end(); ++it)
        {
            if ((*it)->_Type == "Rank") (*it)->setPixmap(*_RankPic[_Rank]);
            if ((*it)->_Type == "RainbowNum") (*it)->changeText("x " + QString::number(_RainbowHit));
            if ((*it)->_Type == "NormalNum") (*it)->changeText("x " + QString::number(_NormalHit));
            if ((*it)->_Type == "MissNum") (*it)->changeText("x " + QString::number(_Miss));
            if ((*it)->_Type == "ComboNum") (*it)->changeText("MaxCombo: " + QString::number(_MaxCombo));
            if ((*it)->_Type.back() > '9'|| (*it)->_Type[0] < '0') (*it)->show_with_raise();
            if ((*it)->_Type.back() == D + '0') (*it)->show_with_raise(); // Show the DifficultyBoard
            if ((*it)->_Type == "BeginnerModeB4" && _ifBeginner) (*it)->show_with_raise();
        }
        for (auto it = _SongMenuBTN.begin(); it != _SongMenuBTN.end(); ++it)
        {
            if ((*it)->_Type == "Back")
            {
                (*it)->setGeometry(1600, 900, 250, 100);
                (*it)->show();
            }
            if ((*it)->_Type == "Restart")
                (*it)->show();
        }
    }
    if (_Command == "Hide")
    {
        for (auto it = _SongMenuBTN.begin(); it != _SongMenuBTN.end(); ++it)
            (*it)->hide();
        for (auto it = _ScoreTextBoard.begin(); it != _ScoreTextBoard.end(); ++it)
            (*it)->hide();
        for (auto it = _PlayBoard.begin(); it != _PlayBoard.end(); ++it)
            (*it)->hide();
    }
}

void UI::PlaySong(QString _Command) // Start the Play
{
    const auto C = _SongList[_CurrentSong];
    const qint32 D = _Difficulty;
    const qint32 N = C->_NoteNum[D];
    if (_Command == "Show")
    {
        _CurrentScene = "PlaySong";  // Initialize the play
        _BackGround->setAlpha(0.5);
        _RainbowHit = 0;
        _NormalHit = 0;
        _Miss = 0;
        _Combo = 0;
        _MaxCombo = 0;
        _NoteScore = 0;
        _ComboScore = 0;
        _TotalScore = 0;
        _CurrentSongTime = 0;
        _AvgNote = 980000 / N;
        _ComboLow = 5000 / (N * 0.2);
        _ComboHigh = (30000 / (N * 0.2) - 2 * _ComboLow) / (N * 0.2 - 1);
        _Num = 1;
        _BGM->setMedia(QUrl::fromLocalFile(C->_Path + "/" + C->_MusicName));
        RefreshScore();
        for (auto it = _NoteQueue.begin(); it != _NoteQueue.end(); ++it)
            (*it)->clear();
        for (auto it = _PlayBoard.begin(); it != _PlayBoard.end(); ++it)
        {
            if ((*it)->_Type.back() == D + '0') (*it)->show_with_raise(); // Show the DifficultyBoard
            if ((*it)->_Type == "BeginnerModeB4" && _ifBeginner) (*it)->show_with_raise();
            if ((*it)->_Type == "SongName")
            {
                (*it)->changeText(C->_Name);
                (*it)->show_with_raise();
            }
        }
        _PauseRBTN->show_with_raise();
        _BGM->setVolume(50);
        _BGM->play();
        connect(_BGM, &QMediaPlayer::mediaStatusChanged, this, [=]
        {
            if (_BGM->mediaStatus() != QMediaPlayer::BufferedMedia) return;
            _CurrentSongLength = C->_Length;
            _SongTimer.start(_CurrentSongLength);
            _NoteTimer.start(16);
            _JudgeTimer.start(5);
            connect(&_JudgeTimer, &QTimer::timeout, this, [=]
            {
                if (_CurrentSongTime >= C->_Length) return;
                _CurrentSongTime = C->_Length - _SongTimer.remainingTime();
            });
            connect(&_NoteTimer, &QTimer::timeout, this, [=]
            {
                if (_Num > C->_NoteNum[D]) return;
                if (1000 * C->_NoteTime[D][_Num] - 700 > _CurrentSongTime + _MusicOffset) return;
                _NoteQueue[C->_Colour[D][_Num]]->enqueue(new Note(this, C->_NoteStyle[D][_Num],
                    _Num, C->_Colour[D][_Num], C->x[D][_Num], C->y[D][_Num]));
                _NoteQueue[C->_Colour[D][_Num]]->back()->play();
                _Num ++;
            });
            connect(&_SongTimer, &QTimer::timeout, this, [=]
            {
                disconnect(&_SongTimer, nullptr, this, nullptr);
                Transfer("PlaySong", "Result");     
            });
            disconnect(_BGM, nullptr, this, nullptr);
        });
    }
    if (_Command == "Hide")
    {
        CleanStage();
        _NoteTimer.stop();
        _JudgeTimer.stop();
        _SongTimer.stop();
        _BGM->stop();
        _PauseRBTN->hide();
        for (auto it = _SongMenuBTN.begin(); it != _SongMenuBTN.end(); ++it)
            (*it)->hide();
        for (auto it = _ScoreTextBoard.begin(); it != _ScoreTextBoard.end(); ++it)
            (*it)->hide();
        for (auto it = _PlayBoard.begin(); it != _PlayBoard.end(); ++it)
            (*it)->hide();
        disconnect(&_NoteTimer, nullptr, this, nullptr);
        disconnect(&_SongTimer, nullptr, this, nullptr);
        disconnect(&_JudgeTimer, nullptr, this, nullptr);
    }
}

void UI::SongMenu(QString _Command) // Show the SongMenu
{
    if (_Command == "Show")
    {
        _CurrentScene = "SongMenu";
        _SongNumTitle->changeText(QString::number(_CurrentSong + 1) + " / " + QString::number(_SongList.size()));
        _SongNumTitle->show_with_raise();
        _BackGround->setAlpha(0.5);
        _SongPicture->show();
        for (auto it = _SongMenuBTN.begin(); it != _SongMenuBTN.end(); ++it)
        {
            if ((*it)->_Type == "Restart") continue;
            if ((*it)->_Type == "Back")
                (*it)->move(80, 70);
            if ((*it)->_Type == "Editer")
                (*it)->setAvailable(false);
            (*it)->show();
        }
        for (auto it = _SongNameBoard.begin(); it != _SongNameBoard.end(); ++it)
            (*it)->show();
        for (auto it = _SongInfo.begin(); it != _SongInfo.end(); ++it)
            (*it)->show_with_raise();
        Get_Current_Song("Show");
    }
    if (_Command == "Hide")
    {
        _BackGround->setAlpha(1);
        _SongPicture->hide();
        _SongNumTitle->hide();
        for (auto it = _SongNameBoard.begin(); it != _SongNameBoard.end(); ++it)
            (*it)->hide();
        for (auto it = _SongInfo.begin(); it != _SongInfo.end(); ++it)
            (*it)->hide();
        for (auto it = _SongMenuBTN.begin(); it != _SongMenuBTN.end(); ++it)
            (*it)->hide();
    }
}

void UI::MainTitle(QString _Command) // Show the MainTitle
{
    if (_Command == "Show")
    {
        _CurrentScene = "MainTitle";
        _BackGround->setPixmap(*_MainTitle);
        _PressEnterUI->show_with_raise();
        _Author->show_with_raise();
        _BGM->setMedia(QUrl("qrc:/sound/TitleBGM.mp3"));
        _BGM->setVolume(50);
        _BGM->play();
    }
    if (_Command == "Hide")
    {
        _PressEnterUI->hide();
        _Author->hide();
    }
}

void UI::Get_Current_Song(QString _Command) // Get the song's info
{
    static qint32 _VSign;
    static qint32 _HSign;
    const auto C = _SongList[_CurrentSong];
    const auto CB = &_CenterBoardNum;
    const qint32 M = _SongList.size();
    _SwitchTime = 0;
    if (_Command == "Up" || _Command == "Down") // Make the switching animation
    {
        if (_MouseInfo != "NULL" && C->_Difficulty[Data::difficulty(_MouseInfo)] == "1")
            Get_Song_Info(_MouseInfo);
        else Get_Song_Info("Hide");
        if  (_Command == "Down")
        {
            (*CB) ++;
            (*CB) %= 8;
            _SongNameBoard[(((*CB) + 3) % 8)]->changeText(_SongList[(_CurrentSong + 3 * M + 3) % M]->_Name);
            if (_SongList[(_CurrentSong + 3 * M + 3) % M]->_Name.size() > 15)
                _SongNameBoard[((*CB) + 3) % 8]->setFont(Data::font(1, 80, 30));
            else _SongNameBoard[((*CB) + 3) % 8]->setFont(Data::font(1, 100, 30));
            _SongNameBoard[((*CB) + 3) % 8]->move(-220, 940);
            _SongNameBoard[((*CB) + 3) % 8]->setAlpha(-0.1);
            _HSign = -1;
            _VSign =  1;
        }
        if (_Command == "Up")
        {
            (*CB) += 7;
            (*CB) %= 8;
            _SongNameBoard[(((*CB) + 5) % 8)]->changeText(_SongList[(_CurrentSong + 3 * M - 3) % M]->_Name);
            if (_SongList[(_CurrentSong + 3 * M - 3) % M]->_Name.size() > 15)
                _SongNameBoard[((*CB) + 5) % 8]->setFont(Data::font(1, 80, 30));
            else _SongNameBoard[((*CB) + 5) % 8]->setFont(Data::font(1, 100, 30));
            _SongNameBoard[(((*CB) + 5) % 8)]->move(-220, -20);
            _SongNameBoard[((*CB) + 5) % 8]->setAlpha(-0.1);
            _HSign = 1;
            _VSign = -1;
        }
        for (qint32 k = 0; k < 8; ++k)
        {
            if (k == (*CB) - 2 || k == (*CB) + 6) _SongNameBoard[k]->raise();
            if (k == (*CB) + 2 || k == (*CB) - 6) _SongNameBoard[k]->raise();
        }
        for (qint32 k = 0; k < 8; ++k)
        {
            if (k == (*CB) - 1 || k == (*CB) + 7) _SongNameBoard[k]->raise();
            if (k == (*CB) + 1 || k == (*CB) - 7) _SongNameBoard[k]->raise();
        }
        for (qint32 k = 0; k < 8; ++k)
        {
            if (k == (*CB))
            {
                _SongNameBoard[(*CB)]->raise();
                _SongNameBoard[(*CB)]->setStyleSheet("background-color: #ffcc00;color:white;");
            }
            else _SongNameBoard[k]->setStyleSheet("background-color: #7d89ff;color:white;");
        }
        _SwitchTimer.start(3);
        connect(&_SwitchTimer, &QTimer::timeout, this, [=]
        {
            qint32 _Sign;
            _SwitchTime += 0.1;
            if (_SwitchTime <= 1.03)
            {
                for (qint32 k = 0; k < 8; ++k)
                {
                    if (_Command == "Down")
                    {
                        if ((k >= (*CB) - 4 && k < (*CB)) || k >= (*CB) + 4) _Sign = - _VSign;
                        else _Sign = _VSign;
                    }
                    else
                    {
                        if ((k > (*CB) && k <= (*CB) + 4) || k <= (*CB) - 4) _Sign = _VSign;
                        else _Sign = - _VSign;
                    }
                    _SongNameBoard[k]->setAlpha(_SongNameBoard[k]->_Alpha + _Sign * 0.025);
                    _SongNameBoard[k]->setGeometry(_SongNameBoard[k]->x() + _Sign * 8, _SongNameBoard[k]->y() + _HSign * 12, 880, 150);
                }
                return;
            }
            disconnect(&_SwitchTimer, nullptr, this, nullptr);
            _SwitchTimer.stop();
            this->Get_Current_Song("Show"); // The switch animation has been finished.
        });
    }
    if (_Command == "Show")
    {
        _SongNumTitle->changeText(QString::number(_CurrentSong + 1) + " / " + QString::number(M));
        for (auto it = _SongNameBoard.begin(); it != _SongNameBoard.end(); ++it)
            (*it)->show();
        _BackGround->setPixmap(C->_BackGroundPic);
        _SongPicture->setPixmap(C->_SmallPic);
        _BGM->setMedia(QUrl::fromLocalFile(C->_Path + "/" + C->_MusicName));
        _BGM->setVolume(50);
        _BGM->play();
        connect(_BGM, &QMediaPlayer::metaDataAvailableChanged, this, [=]
        {
            qint32 _Minute, _Second;
            QString _ZeroM, _ZeroS;
            C->_Length = _BGM->metaData("Duration").toInt();
            _Second = C->_Length / 1000;
            _Minute = _Second / 60;
            _Second %= 60;
            _ZeroM = _Minute > 9 ? "" : "0";
            _ZeroS = _Second > 9 ? "" : "0";
            _SongInfo[0]->changeText("原作 :  " + C->_Author);
            _SongInfo[1]->changeText("谱师 :  " + C->_Mapper);
            _SongInfo[2]->changeText("时长 :  " + _ZeroM + QString::number(_Minute) + " : " + _ZeroS + QString::number(_Second));
            _SongInfo[3]->changeText("BPM :  " + QString::number(C->_BPM));
            disconnect(_BGM, nullptr, this, nullptr);
            return;
        });
        for (qint32 k = 5; k <= 8; ++k)
            C->_Difficulty[k - 5] == "1" ? _SongMenuBTN[k]->setAvailable(true) : _SongMenuBTN[k]->setAvailable(false);
        for (auto it = _SongMenuBTN.begin(); it != _SongMenuBTN.end(); ++it)
            (*it)->raise();  
    }
}

void UI::Get_Song_Info(QString _Command) // Get the detailed Song's info when the mouse is over the button
{
    const auto C = _SongList[_CurrentSong];
    if (_Command != "Hide")
    {
        for (auto it = _StarBoard.begin(); it != _StarBoard.end(); ++it)
            (*it)->hide();
        disconnect(&_Brown->_Timer, nullptr, this, nullptr);
        _Brown->_Timer.start(10);
        _Brown->_Time = 0;
        RefreshRankList(_Command);
        connect(&_Brown->_Timer, &QTimer::timeout, this, [=]
        {
            _Brown->_Time += 0.05;
            if (_Brown->_Time  <= 1.03)
            {
                _Brown->move(qint32(-1000 * qExp((-5) * _Brown->_Time) + 6), _Brown->y());
                _HighScoreName->textmove(qint32(-1000 * qExp((-5) * _Brown->_Time) + 416), 185);
                _HighScore->textmove(qint32(-1000 * qExp((-5) * _Brown->_Time) + 686), 185);
                _HighScoreRank->move(qint32(-1000 * qExp((-5) * _Brown->_Time) + 806), _HighScoreRank->y());
                _Brown->show_with_raise();
                _HighScoreName->show_with_raise();
                _HighScore->show_with_raise();
                _HighScoreRank->show_with_raise();
                for (qint32 k = 0; k < C->_Level[Data::difficulty(_Command)]; ++k)
                {
                    _StarBoard[k]->move(qint32(-1000 * qExp((-5) * _Brown->_Time) + 287 + 45 * k), 30);
                    _StarBoard[k]->show_with_raise();
                }
                for (qint32 k = 1; k <= 10; ++k)
                {
                    _RankList[k]->move(qint32(-1000 * qExp((-5) * _Brown->_Time) + 6), 0);
                    ++(*_RankList[k]);
                }
                return;
            }
            _Brown->_Timer.stop();
            disconnect(&_Brown->_Timer, nullptr, this, nullptr);
        });
    }
    if (_Command == "Hide")
    {
        disconnect(&_Brown->_Timer, nullptr, this, nullptr);
        if (_MouseInfo != "NULL") _MouseInfo = "NULL";
        _Brown->_Timer.start(10);
        connect(&_Brown->_Timer, &QTimer::timeout, this, [=]
        {
            _Brown->_Time -= 0.05;
            if (_Brown->_Time  >= -0.02)
            {
                _Brown->move(qint32(-1000 * qExp((-5) * _Brown->_Time) + 6), _Brown->y());
                _HighScoreName->textmove(qint32(-1000 * qExp((-5) * _Brown->_Time) + 416), 185);
                _HighScore->textmove(qint32(-1000 * qExp((-5) * _Brown->_Time) + 686), 185);
                _HighScoreRank->move(qint32(-1000 * qExp((-5) * _Brown->_Time) + 806), _HighScoreRank->y());
                _Brown->show_with_raise();
                _HighScoreName->show_with_raise();
                _HighScore->show_with_raise();
                _HighScoreRank->show_with_raise();
                for (qint32 k = 0; k < _StarBoard.size(); ++k)
                {
                    _StarBoard[k]->move(qint32(-1000 * qExp((-5) * _Brown->_Time) + 287 + 45 * k), 30);
                    _StarBoard[k]->raise();
                }
                for (qint32 k = 1; k <= 10; ++k)
                {
                    _RankList[k]->move(qint32(-1000 * qExp((-5) * _Brown->_Time) + 6), 0);
                    ++(*_RankList[k]);
                }
                return;
            }
            _Brown->_Timer.stop();
            _Brown->hide();
            _HighScoreName->hide();
            _HighScore->hide();
            _HighScoreRank->hide();
            for (auto it = _StarBoard.begin(); it != _StarBoard.end(); ++it)
                (*it)->hide();
            for (qint32 k = 1; k <= 10; ++k)
                --(*_RankList[k]);
            disconnect(&_Brown->_Timer, nullptr, this, nullptr);
        });
    }
}

void UI::Transfer(QString _Scene) // Show the scene
{
    static qreal now_percent = 0, max_percent = 30;
    SwitchScene("Show", _Scene);
    _Transfer->show_with_raise();
    _Transfer->_Timer.start(10);
    connect(&_Transfer->_Timer, &QTimer::timeout, this, [=]
    {
        if (now_percent <= 30)
        {
            _Transfer->setAlpha(1 - now_percent / max_percent);
            now_percent ++;
            return;
        }
        now_percent = 0;
        _Transfer->_Timer.stop();
        _Transfer->hide();
        disconnect(&_Transfer->_Timer, nullptr, this, nullptr);
        if (_CurrentScene == "Result")
        {
            Loading("Connecting");
            QtConcurrent::run(SqlConnect::UploadScore);;
        }
        if (_CurrentScene == "SongMenu")
        {
            Loading("Connecting");
            QtConcurrent::run(SqlConnect::DownloadScore);
        }
    });
}

void UI::Transfer(QString _SceneA, QString _SceneB) // Hide one scene and show another
{
    static qreal now_percent = 0, max_percent = 30;
    _Transfer->show_with_raise();
    _Transfer->_Timer.start(10);
    connect(&_Transfer->_Timer, &QTimer::timeout, this, [=]
    {
        if (now_percent <= 30)
        {
            _Transfer->setAlpha(now_percent / max_percent);
            now_percent ++;
            return;
        }
        now_percent = 0;
        _Transfer->_Timer.stop();
        disconnect(&_Transfer->_Timer, nullptr, this, nullptr);
        SwitchScene("Hide", _SceneA);
        _Transfer->hide();
        Transfer(_SceneB);
    });
}

void UI::SwitchScene(QString _Command, QString _Scene) //Switch the scene
{
    if (_Scene == "MainTitle") MainTitle(_Command);
    if (_Scene == "SongMenu") SongMenu(_Command);
    if (_Scene == "PlaySong") PlaySong(_Command);
    if (_Scene == "Result") Result(_Command);
}

void UI::Load_Beatmap() // Load all the beatmaps and playing infos
{
    QDir _SongContent;
    QString _RootContent;
    QStringList _FilterMusic;
    if (!_SongContent.exists(_SongContent.currentPath() + "/Songs"))
    {
       _ErrorNum = 1;
       return;
    }
    _FilterMusic << "*.mp3" << "*.wma" << "*.wav";
    _SongContent = _SongContent.absolutePath() + "/Songs";
    _RootContent = _SongContent.absolutePath();
    foreach(QDir _CurrentSongPack, _SongContent.entryList())
    {
        if (_CurrentSongPack.dirName() == "." || _CurrentSongPack.dirName() == "..") continue;
        Beatmap *_NewMap = new Beatmap;
        _NewMap->_Path = _RootContent + "/" + _CurrentSongPack.dirName();
        _NewMap->_Name = _CurrentSongPack.dirName();
        _NewMap->_BackGroundPic.load(_NewMap->_Path + "/" + _NewMap->_Name + "_B");
        _NewMap->_SmallPic.load(_NewMap->_Path + "/" + _NewMap->_Name);
        _SongContent = _NewMap->_Path;
        _SongContent.setNameFilters(_FilterMusic);
        foreach(QFileInfo _CurrentFile, _SongContent.entryList())
            _NewMap->_MusicName = _CurrentFile.fileName();
        QFile _Mapdata(_NewMap->_Path + "/" + _NewMap->_Name + ".tsm");
        _Mapdata.open(QFile::ReadOnly);
        QTextStream _Read(&_Mapdata);
        QString _Style;
        _NewMap->_Author = Data::readS(_Read);
        _NewMap->_Mapper = Data::readS(_Read);
        _Read >> _NewMap->_BPM;
        _NewMap->_Difficulty = Data::readS(_Read);
        for (qint32 k = 0; k < 4; ++k)
        {
            if (_NewMap->_Difficulty[k] == "0") continue;
            _Read >> _NewMap->_NoteNum[k] >> _NewMap->_Level[k];
            _NewMap->x[k] = new qint32[_NewMap->_NoteNum[k]+1];
            _NewMap->y[k] = new qint32[_NewMap->_NoteNum[k]+1];
            _NewMap->_NoteTime[k] = new qreal[_NewMap->_NoteNum[k]+1];
            _NewMap->_Colour[k] = new qint32[_NewMap->_NoteNum[k]+1];
            _NewMap->_Constant[k] = new qint32[_NewMap->_NoteNum[k]+1];
            for (qint32 i = 1; i <= _NewMap->_NoteNum[k]; ++i)
            {
                _Read >> _NewMap->x[k][i] >> _NewMap->y[k][i] >> _Style >> _NewMap->_NoteTime[k][i] >> _NewMap->_Constant[k][i];
                _NewMap->_NoteStyle[k].push_back(_Style);
                if (_Style[0] == 'R') _NewMap->_Colour[k][i] = 0;
                if (_Style[0] == 'B') _NewMap->_Colour[k][i] = 1;
                if (_Style[0] == 'G') _NewMap->_Colour[k][i] = 2;
            }
        }
        QString _MapPassword;
        QFile _ScoreDataContent(_NewMap->_Path + "/" + _NewMap->_Name + ".tsd");
        _MapPassword = Data::getMD5(&_Mapdata);
        _NewMap->_MapHash = _MapPassword;
        if (_ScoreDataContent.exists(_NewMap->_Path + "/" + _NewMap->_Name + ".tsd"))
        {
            if (!Data::checkID(&_ScoreDataContent, _NewMap->_MapHash))
            {
                _ScoreDataContent.open(QFile::WriteOnly);
                _ScoreDataContent.flush();
                _ScoreDataContent.close();
                Data::writeScore(&_ScoreDataContent, _NewMap->_MapHash);
                for (qint32 k = 0; k < 4; ++k)
                {
                    Data::writeScore(&_ScoreDataContent, "还没打过哟");
                    Data::writeScore(&_ScoreDataContent, "0");
                }
            }
        }
        else
        {
            Data::writeScore(&_ScoreDataContent, _MapPassword);
            for (qint32 k = 0; k < 4; ++k)
            {
                Data::writeScore(&_ScoreDataContent, "还没打过哟");
                Data::writeScore(&_ScoreDataContent, "0");
            }
        }
        _ScoreDataContent.open(QFile::ReadOnly);
        for (qint32 k = 0; k < 4; ++k)
        {

            if (k == 0) Data::readScore(&_ScoreDataContent, &_NewMap->_HighScoreName[k], &_NewMap->_HighScore[k], 1);
            else Data::readScore(&_ScoreDataContent, &_NewMap->_HighScoreName[k], &_NewMap->_HighScore[k]);
            if (_NewMap->_HighScore[k].toInt() >= 980000) _NewMap->_HighRank[k] = 0;
            else if (_NewMap->_HighScore[k].toInt() >= 800000) _NewMap->_HighRank[k] = 4 - (_NewMap->_HighScore[k].toInt()- 800000) / 50000;
            else if (_NewMap->_HighScore[k].toInt() >= 600000) _NewMap->_HighRank[k] = 5;
            else _NewMap->_HighRank[k] = 6;
        }
        _SongList.push_back(_NewMap);
        _ScoreDataContent.close();
        _Mapdata.close();
    }
}

void UI::Setting(QString _Command) // Load or save the game setting
{
    QFile _SettingFile("Setting.tss");
    SqlConnect::Initialize();
    if (_Command == "Load")
    {
        _SettingFile.open(QFile::ReadOnly);
        QTextStream _Text(&_SettingFile);
        _Text >> _HitOffset >> _MusicOffset;
        _SettingChar = Data::readS(_Text);
        _Text >> _PlayerName;
    }
    if (_Command == "Save")
    {
        _SettingFile.close();
        _SettingFile.open(QFile::WriteOnly);
        _SettingFile.flush();
        QTextStream _Text(&_SettingFile);
        _Text << _HitOffset << " " << _MusicOffset << '\xd' << '\xa';
        _Text << _SettingChar << '\xd' << '\xa';
        _Text << "Guest" << '\xd' << '\xa';
        for (auto it = _SongList.begin(); it != _SongList.end(); ++it)
        {
            QFile _ScoreDataContent((*it)->_Path + "/" + (*it)->_Name + ".tsd");
            _ScoreDataContent.open(QFile::WriteOnly);
            _ScoreDataContent.flush();
            _ScoreDataContent.close();
            Data::writeScore(&_ScoreDataContent, ((*it)->_MapHash));
            for (qint32 k = 0; k < 4; ++k)
            {
                Data::writeScore(&_ScoreDataContent, ((*it)->_HighScoreName[k]));
                Data::writeScore(&_ScoreDataContent, ((*it)->_HighScore[k]));
            }
            _ScoreDataContent.close();
        }
    }
    _SettingFile.close();
}

void UI::Load_Sound() // Load the sound effects
{
    _SwitchSong->setSource(QUrl("qrc:/sound/SwitchSong.wav"));
    _SwitchSong->setVolume(0.5);
    _Click->setSource(QUrl("qrc:/sound/Click.wav"));
    _Enter->setSource(QUrl("qrc:/sound/Enter.wav"));
    _Enter->setVolume(0.5);
}

void UI::SetFont() // Add the fonts
{
    QFontDatabase::addApplicationFont(":/font/FuturaHandwritten.ttf");
    QFontDatabase::addApplicationFont(":/font/SourceHanSansCN-Medium.ttf");
    QFontDatabase::addApplicationFont(":/font/AdobeHeitiStd-Regular.otf");
}

void UI::Initialize() // Initialize some variables
{
    const qint32 M = _SongList.size();
    _Hintnum = 0;
    _CurrentSong = 0;
    _PressEnter->setSpeed(600);
    _isPause = false;
    _iflogin = false;
    _LoadingState = "";
    _SettingChar[5] == 'T' ? _ifHitSound = true : _ifHitSound = false;
    _SettingChar[6] == 'T' ? _ifBeginner = true : _ifBeginner = false;
    _ifChangeKey = 0;
    _MouseInfo = "NULL";
    _PauseRBTN->setAlpha(0.9);
    for (auto it = _SongMenuBTN.begin(); it != _SongMenuBTN.end(); ++it)
        (*it)->setAlpha(0.8);
    _BackStyle->show();
    _BackGround->show();
    _NameBoard->setText(_PlayerName);
    _CenterBoardNum = 3;
    if (_ErrorNum == 1) return;
    for (qint32 k = 0; k < 8; ++k)
    {
        _SongNameBoard[k]->setStyleSheet("background-color: #7d89ff;color:white;");
        _SongNameBoard[k]->setAlpha(0.9 + qFabs(k - 3) * (- 0.25));
        _SongNameBoard[k]->changeText(_SongList[(_CurrentSong + 3 * M + k - 3) % M]->_Name);
        if (_SongList[(_CurrentSong + 3 * M + k - 3) % M]->_Name.size() > 15)
            _SongNameBoard[k]->setFont(Data::font(1, 80, 30));
        else _SongNameBoard[k]->setFont(Data::font(1, 100, 30));
        _SongNameBoard[k]->setGeometry(100 + qAbs(k - 3) * (- 80), 100 + 120 * k, 880, 150);
    }
    _SongNameBoard[5]->raise();
    _SongNameBoard[4]->raise();
    _SongNameBoard[3]->raise();
    _SongNameBoard[3]->setStyleSheet("background-color: #ffcc00;color:white;");
    for (qint32 k = 0; k < 4 * _SongList.size(); ++k) // Initialize the rankinfo
    {
        PlayInfo _Blank;
        for (qint32 i = 0; i <= 10; ++i)
        {
            _Blank._Name[i] = "";
            _Blank._Score[i] = "0";
            _Blank._Date[i] = "";
        }
        _RankInfo.push_back(_Blank);
    }
}

UI::UI(MainWindow *screen)
    :QObject()
{
    SqlConnect::instance()->_UI = this;
    _Screen = screen;
    _ErrorNum = 0;
    _LastHint = nullptr;
    SetFont();
    _NoteQueue.push_back(new QQueue<Note*>());
    _NoteQueue.push_back(new QQueue<Note*>());
    _NoteQueue.push_back(new QQueue<Note*>());
    _EscapeEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
    _ReturnEvent = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
    _MainTitle = new QPixmap(":/image/Title.png");
    _Black = new QPixmap(":/image/Black.png");
    _back = new QPixmap(":/image/back.png");
    _restart = new QPixmap(":/image/restart.png");
    _quit = new QPixmap(":/image/quit.png");
    _help = new QPixmap(":/image/Help.png");
    _editer = new QPixmap(":/image/editer.png");
    _setting = new QPixmap(":/image/setting.png");
    _easy = new QPixmap(":/image/easy.png");
    _normal = new QPixmap(":/image/normal.png");
    _hard = new QPixmap(":/image/hard.png");
    _extreme = new QPixmap(":/image/extreme.png");
    _EasyB = new QPixmap(":/image/EasyB.png");
    _NormalB = new QPixmap(":/image/NormalB.png");
    _HardB = new QPixmap(":/image/HardB.png");
    _ExtremeB = new QPixmap(":/image/ExtremeB.png");
    _BeginnerMode = new QPixmap(":/image/BeginnerMode.png");
    _NotePic.push_back(new QPixmap(":/image/Red1.png"));
    _NotePic.push_back(new QPixmap(":/image/Red2.png"));
    _NotePic.push_back(new QPixmap(":/image/Red3.png"));
    _NotePic.push_back(new QPixmap(":/image/Blue1.png"));
    _NotePic.push_back(new QPixmap(":/image/Blue2.png"));
    _NotePic.push_back(new QPixmap(":/image/Blue3.png"));
    _NotePic.push_back(new QPixmap(":/image/Green1.png"));
    _NotePic.push_back(new QPixmap(":/image/Green2.png"));
    _NotePic.push_back(new QPixmap(":/image/Green3.png"));
    _NotePic.push_back(new QPixmap(":/image/Red1_L.png"));
    _NotePic.push_back(new QPixmap(":/image/Red2_L.png"));
    _NotePic.push_back(new QPixmap(":/image/Red3_L.png"));
    _NotePic.push_back(new QPixmap(":/image/Blue1_L.png"));
    _NotePic.push_back(new QPixmap(":/image/Blue2_L.png"));
    _NotePic.push_back(new QPixmap(":/image/Blue3_L.png"));
    _NotePic.push_back(new QPixmap(":/image/Green1_L.png"));
    _NotePic.push_back(new QPixmap(":/image/Green2_L.png"));
    _NotePic.push_back(new QPixmap(":/image/Green3_L.png"));
    _NotePic.push_back(new QPixmap(":/image/Rainbow1.png"));
    _NotePic.push_back(new QPixmap(":/image/Rainbow2.png"));
    _NotePic.push_back(new QPixmap(":/image/Rainbow3.png"));
    _RankPic.push_back(new QPixmap(":/image/Astar.png"));
    _RankPic.push_back(new QPixmap(":/image/Aplus.png"));
    _RankPic.push_back(new QPixmap(":/image/A.png"));
    _RankPic.push_back(new QPixmap(":/image/Bplus.png"));
    _RankPic.push_back(new QPixmap(":/image/B.png"));
    _RankPic.push_back(new QPixmap(":/image/C.png"));
    _RankPic.push_back(new QPixmap(":/image/F.png"));
    _NumPic.push_back(new QPixmap(":/image/0.png"));
    _NumPic.push_back(new QPixmap(":/image/1.png"));
    _NumPic.push_back(new QPixmap(":/image/2.png"));
    _NumPic.push_back(new QPixmap(":/image/3.png"));
    _NumPic.push_back(new QPixmap(":/image/4.png"));
    _NumPic.push_back(new QPixmap(":/image/5.png"));
    _NumPic.push_back(new QPixmap(":/image/6.png"));
    _NumPic.push_back(new QPixmap(":/image/7.png"));
    _NumPic.push_back(new QPixmap(":/image/8.png"));
    _NumPic.push_back(new QPixmap(":/image/9.png"));
    _NumPic.push_back(new QPixmap(":/image/00.png"));
    _NumPic.push_back(new QPixmap(":/image/11.png"));
    _NumPic.push_back(new QPixmap(":/image/22.png"));
    _NumPic.push_back(new QPixmap(":/image/33.png"));
    _NumPic.push_back(new QPixmap(":/image/44.png"));
    _NumPic.push_back(new QPixmap(":/image/55.png"));
    _NumPic.push_back(new QPixmap(":/image/66.png"));
    _NumPic.push_back(new QPixmap(":/image/77.png"));
    _NumPic.push_back(new QPixmap(":/image/88.png"));
    _NumPic.push_back(new QPixmap(":/image/99.png"));
    _PausePic.push_back(new QPixmap(":/image/Back_C.png"));
    _PausePic.push_back(new QPixmap(":/image/Back_CL.png"));
    _PausePic.push_back(new QPixmap(":/image/Restart_C.png"));
    _PausePic.push_back(new QPixmap(":/image/Restart_CL.png"));
    _PausePic.push_back(new QPixmap(":/image/Menu_C.png"));
    _PausePic.push_back(new QPixmap(":/image/Menu_CL.png"));
    _PausePic.push_back(new QPixmap(":/image/Quit_C.png"));
    _PausePic.push_back(new QPixmap(":/image/Quit_CL.png"));
    _PausePic.push_back(new QPixmap(":/image/PauseR.png"));
    _PausePic.push_back(new QPixmap(":/image/PauseR_L.png"));
    _FunctionPic.push_back(new QPixmap(":/image/Yes.png"));
    _FunctionPic.push_back(new QPixmap(":/image/Yes_L.png"));
    _FunctionPic.push_back(new QPixmap(":/image/Cancel.png"));
    _FunctionPic.push_back(new QPixmap(":/image/Cancel_L.png"));
    _FunctionPic.push_back(new QPixmap(":/image/Login.png"));
    _FunctionPic.push_back(new QPixmap(":/image/Login_L.png"));
    _FunctionPic.push_back(new QPixmap(":/image/Register.png"));
    _FunctionPic.push_back(new QPixmap(":/image/Register_L.png"));
    _FunctionPic.push_back(new QPixmap(":/image/ChangeHeadportrait.png"));
    _FunctionPic.push_back(new QPixmap(":/image/ChangeHeadportrait_L.png"));
    _FunctionPic.push_back(new QPixmap(":/image/Logout.png"));
    _FunctionPic.push_back(new QPixmap(":/image/Logout_L.png"));
    _RainboWE = new QPixmap(":/image/RainbowE.png");
    _NormalE = new QPixmap(":/image/NormalE.png");
    _MissE = new QPixmap(":/image/MissE.png");
    _Pause = new QPixmap(":/image/Pause.png");
    _IfQuit = new QPixmap(":/image/ifquit.png");
    _Settings = new QPixmap(":/image/settings.png");
    _HelpBoard = new QPixmap(":/image/HelpBoard.png");
    _LeftArrow = new QPixmap(":/image/left_arrow.png");
    _LeftArrow_L = new QPixmap(":/image/left_arrow_L.png");
    _RightArrow = new QPixmap(":/image/right_arrow.png");
    _RightArrow_L = new QPixmap(":/image/right_arrow_L.png");
    _Settingbtn = new QPixmap(":/image/SettingBTN.png");
    _Settingbtn_L = new QPixmap(":/image/SettingBTN_L.png");
    _LevelBoard = new QPixmap(":/image/LevelBoard.png");
    _LevelStar1 = new QPixmap(":/image/LevelStar1.png");
    _LevelStar2 = new QPixmap(":/image/LevelStar2.png");
    _LevelStar3 = new QPixmap(":/image/LevelStar3.png");
    _LevelStar4 = new QPixmap(":/image/LevelStar4.png");
    _question = new QPixmap(":/image/question.png");
    _HelpOfKey = new QPixmap(":/image/HelpOfKey.png");
    _HelpBeginner = new QPixmap(":/image/HelpBeginner.png");
    _HelpBeatmap = new QPixmap(":/image/HelpBeatmap.png");
    _LoginScreen = new QPixmap(":/image/LoginAccount.png");
    _RegisterScreen = new QPixmap(":/image/RegisterAccount.png");
    _Connecting = new QPixmap(":/image/Connecting.png");
    _LoadingL = new QPixmap(":/image/LoadingL.png");
    _PressEnter = new QMovie(":/image/Press_Enter_To_Start.gif");
    _BGM = new QMediaPlayer(this, QMediaPlayer::LowLatency);
    _SwitchSong = new QSoundEffect(this);
    _Click = new QSoundEffect(this);
    _Enter = new QSoundEffect(this);
    _SongTimer.setTimerType(Qt::PreciseTimer);
    _NoteTimer.setTimerType(Qt::PreciseTimer);
    _Author = new Item(this, "NormalText", "2019     By     一条咸鱼           特别鸣谢：Spitlight", "white", Data::font(1, 30), 350, 1030);
    _BackStyle =new Item(this, "NormalPictrue", _Black, 0, 0, 1920, 1080);
    _BackGround = new Item(this, "NormalPictrue", _MainTitle, 0, 0, 1920, 1080);
    _PressEnterUI = new Item(this, "NormalMovie", _PressEnter, 470, 850, 980, 100);
    _Transfer = new Item(this, "NormalPictrue", _Black, 0, 0, 1920, 1080);
    _SongPicture = new Item(this, "NormalPictrue", _Black, 1000, 240, 400, 400);
    _ConnectingScreen = new Item(this, "LoadingScreen", _Connecting, 0, 0, 1920, 1080);
    _LoadingSign = new Item(this, "LoadingSign", _LoadingL, 1750, 20, 150, 150, -2);
    _SongInfo.push_back(new Item(this, "NormalText", "原作：", "white", Data::font(1, 40), 1440, 250, -2));
    _SongInfo.push_back(new Item(this, "NormalText", "谱师：", "white", Data::font(1, 40), 1440, 360, -2));
    _SongInfo.push_back(new Item(this, "NormalText", "时长：", "white", Data::font(1, 40), 1440, 470, -2));
    _SongInfo.push_back(new Item(this, "NormalText", "BPM：", "white", Data::font(1, 40), 1440, 580, -2));
    _SongNumTitle = new Item(this, "NormalText", " ", "white",Data::font(1, 80, 30), 480, 130);
    _PlayBoard.push_back(new Item(this, "EasyB0", _EasyB, 11, 930, 250, 150));
    _PlayBoard.push_back(new Item(this, "NormalB1", _NormalB, 27, 930, 350, 150));
    _PlayBoard.push_back(new Item(this, "HardB2", _HardB, 21, 930, 250, 150));
    _PlayBoard.push_back(new Item(this, "ExtremeB3", _ExtremeB, 15, 930, 400, 150));
    _PlayBoard.push_back(new Item(this, "BeginnerModeB4", _BeginnerMode, 43, 900, 360 ,90));
    _PlayBoard.push_back(new Item(this, "SongName", " ", "white", Data::font(0, 50), 50, 30, -2));
    _PlayBoard.push_back(new Item(this, "Rank", _Black, 250, 250, 600, 600));
    _PlayBoard.push_back(new Item(this, "RainbowPic", _RainboWE, 1000, 480, 150 ,150));
    _PlayBoard.push_back(new Item(this, "NormalPic", _NormalE, 1000, 630, 150, 150));
    _PlayBoard.push_back(new Item(this, "MissPic", _MissE, 1000, 780, 150, 150));
    _PlayBoard.push_back(new Item(this, "RainbowNum", " ", "white", Data::font(0, 90), 1320, 550));
    _PlayBoard.push_back(new Item(this, "NormalNum", " ", "white", Data::font(0, 90), 1320, 700));
    _PlayBoard.push_back(new Item(this, "MissNum", " ", "white", Data::font(0, 90), 1320, 850));
    _PlayBoard.push_back(new Item(this, "ComboNum", " ", "white", Data::font(0, 60), 1470, 320));
    _StarBoard.push_back(new Item(this, "NormalPicture", _LevelStar1, 280, 30, 60, 60));
    _StarBoard.push_back(new Item(this, "NormalPicture", _LevelStar1, 330, 30, 60, 60));
    _StarBoard.push_back(new Item(this, "NormalPicture", _LevelStar1, 380, 30, 60, 60));
    _StarBoard.push_back(new Item(this, "NormalPicture", _LevelStar2, 430, 30, 60, 60));
    _StarBoard.push_back(new Item(this, "NormalPicture", _LevelStar2, 480, 30, 60, 60));
    _StarBoard.push_back(new Item(this, "NormalPicture", _LevelStar2, 530, 30, 60, 60));
    _StarBoard.push_back(new Item(this, "NormalPicture", _LevelStar3, 580, 30, 60, 60));
    _StarBoard.push_back(new Item(this, "NormalPicture", _LevelStar3, 630, 30, 60, 60));
    _StarBoard.push_back(new Item(this, "NormalPicture", _LevelStar3, 680, 30, 60, 60));
    _StarBoard.push_back(new Item(this, "NormalPicture", _LevelStar4, 730, 30, 60, 60));
    _StarBoard.push_back(new Item(this, "NormalPicture", _LevelStar4, 780, 30, 60, 60));
    _StarBoard.push_back(new Item(this, "NormalPicture", _LevelStar4, 830, 30, 60, 60));
    _SettingText.push_back(new Item(this, "NormalText", " ", "white", Data::font(0, 60), 660, 240));
    _SettingText.push_back(_SettingText[0]);
    _SettingText.push_back(new Item(this, "NormalText", " ", "white", Data::font(0, 60), 660, 420));
    _SettingText.push_back(_SettingText[2]);
    _SettingText.push_back(new Item(this, "NormalText", "A", "black", Data::font(0, 40), 640, 930, 4));
    _SettingText.push_back(new Item(this, "NormalText", "S", "black", Data::font(0, 40), 860, 930, 5));
    _SettingText.push_back(new Item(this, "NormalText", "Spc", "black", Data::font(0, 40), 1180, 930, 6));
    _SettingText.push_back(new Item(this, "NormalText", "K", "black", Data::font(0, 40), 1510, 930, 7));
    _SettingText.push_back(new Item(this, "NormalText", "L", "black", Data::font(0, 40), 1730, 930, 8));
    _SettingText.push_back(new Item(this, "NormalText", "关", "black", Data::font(1, 30, 40), 1610, 250, 9));
    _SettingText.push_back(new Item(this, "NormalText", "开", "black", Data::font(1, 30, 40), 1610, 430, 10));
    _HelpAnswer.push_back(new Item(this, "NormalPicture", _HelpOfKey, 550, 340, 675, 300));
    _HelpAnswer.push_back(new Item(this, "NormalPicture", _HelpBeginner, 500, 470, 675, 250));
    _HelpAnswer.push_back(new Item(this, "NormalPicture", _HelpBeatmap, 650, 590, 675, 200));
    _Gray = new Item(this, "Gray", _Black, 0, 0, 1920, 1080);
    _GrayPlus = new Item(this, "GrayPlus", _Black, 0, 0, 1920, 1080);
    _Brown = new Item(this, "Brown", _LevelBoard, -980, 0, 1000, 1080);
    _PlayerInfo.push_back(new Item(this, "PlayerName", "", "white", Data::font(1, 50, 40), 710, 600));
    _HighScoreName = new Item(this, "NormalPicture", "", "white", Data::font(1, 40, 40), -200, 175);
    _HighScore = new Item(this, "NormalPicture", "", "white", Data::font(1, 60, 40), -200, 180);
    _HighScoreRank = new Item(this, "NormalPicture", _Black, -200, 80, 200, 200);
    for (qint32 k = 0; k < 8; ++k)
    {
        _SongNameBoard.push_back(new Item(this, "NormalText", " ", "white", Data::font(1, 100, 30), 0, 0));
        _ScoreTextBoard.push_back(new Item(this, "NormalPictrue", _Black, 0, 0, 120, 150));
    }
    _NameBoard = new InputBox(this, "NameBoard", 750, 470, 500, 60); // to respond keypressevent
    _PasswordBoard = new InputBox(this, "PasswordBoard", 750, 630, 500, 60);
    for (qint32 k = 0; k <= 10; ++k)
        _RankList.push_back(new ScoreBoard(this, "还没人打过哟", "0" , "", k));
    _SongMenuBTN.push_back(new BTN_EVENT(this, "Back", _back, 80, 70, 250, 100));
    _SongMenuBTN.push_back(new BTN_EVENT(this, "Quit", _quit, 80, 900, 250, 100));
    _SongMenuBTN.push_back(new BTN_EVENT(this, "Editer", _editer, 1000, 80, 250, 100));
    _SongMenuBTN.push_back(new BTN_EVENT(this, "Help", _help, 1300, 80, 250, 100));
    _SongMenuBTN.push_back(new BTN_EVENT(this, "Setting", _setting, 1600, 80, 250, 100));
    _SongMenuBTN.push_back(new BTN_EVENT(this, "Easy", _easy, 1000, 670, 400, 160, 0));
    _SongMenuBTN.push_back(new BTN_EVENT(this, "Normal", _normal, 1440, 670, 400, 160, 1));
    _SongMenuBTN.push_back(new BTN_EVENT(this, "Hard", _hard, 1000, 860, 400, 160, 2));
    _SongMenuBTN.push_back(new BTN_EVENT(this, "Extreme", _extreme, 1440, 860, 400, 160, 3));
    _SongMenuBTN.push_back(new BTN_EVENT(this, "Restart", _restart, 1600, 50, 250, 100));
    _PauseBTN.push_back(new BTN_EVENT(this, "Back_C", _PausePic[0], 320, 400, 300, 300, 0));
    _PauseBTN.push_back(new BTN_EVENT(this, "Restart_C", _PausePic[2], 820, 400, 300, 300, 2));
    _PauseBTN.push_back(new BTN_EVENT(this, "Menu_C", _PausePic[4], 1320, 400, 300, 300, 4));
    _PauseBTN.push_back(new BTN_EVENT(this, "Quit_C", _PausePic[6], 1070, 400, 300, 300, 6));
    _PauseRBTN = new BTN_EVENT(this, "PauseR", _PausePic[8], 1750, 20, 150, 150, 8);
    _SettingBTN.push_back(new BTN_EVENT(this, "H_P", _RightArrow, 680, 160, 150, 150, 0));
    _SettingBTN.push_back(new BTN_EVENT(this, "M_P", _RightArrow, 680, 340, 150, 150, 1));
    _SettingBTN.push_back(new BTN_EVENT(this, "H_M", _LeftArrow, 480, 160, 150, 150, 2));
    _SettingBTN.push_back(new BTN_EVENT(this, "M_M", _LeftArrow, 480, 340, 150, 150, 3));
    _SettingBTN.push_back(new BTN_EVENT(this, "RedKey1", _Settingbtn, 580, 870, 120, 120, 4));
    _SettingBTN.push_back(new BTN_EVENT(this, "RedKey2", _Settingbtn, 800, 870, 120, 120, 5));
    _SettingBTN.push_back(new BTN_EVENT(this, "GreenKey", _Settingbtn, 1120, 870, 120, 120, 6));
    _SettingBTN.push_back(new BTN_EVENT(this, "BlueKey1", _Settingbtn, 1450, 870, 120, 120, 7));
    _SettingBTN.push_back(new BTN_EVENT(this, "BlueKey2", _Settingbtn, 1670, 870, 120, 120, 8));
    _SettingBTN.push_back(new BTN_EVENT(this, "HitSound", _Settingbtn, 1550, 190, 120, 120, 9));
    _SettingBTN.push_back(new BTN_EVENT(this, "BeginnerMode", _Settingbtn, 1550, 370, 120, 120, 10));
    _FunctionBTN.push_back(new BTN_EVENT(this, "Yes", _FunctionPic[0], 710, 800, 120, 120, 0));
    _FunctionBTN.push_back(new BTN_EVENT(this, "Cancel", _FunctionPic[2], 1110, 800, 120, 120, 2));
    _FunctionBTN.push_back(new BTN_EVENT(this, "Login", _FunctionPic[4], 550, 540, 120, 120, 4));
    _FunctionBTN.push_back(new BTN_EVENT(this, "Register", _FunctionPic[6], 750, 540, 120, 120, 6));
    _FunctionBTN.push_back(new BTN_EVENT(this, "ChangeHeadportrait", _FunctionPic[8], 1000, 540, 120, 120, 8));
    _FunctionBTN.push_back(new BTN_EVENT(this, "Logout",_FunctionPic[10], 950, 540, 120, 120, 10));
    _HelpBTN.push_back(new BTN_EVENT(this, "HelpOfKey", _question, 460, 585, 70, 70, 0));
    _HelpBTN.push_back(new BTN_EVENT(this, "HelpBeginner", _question, 420, 660, 70, 70, 1));
    _HelpBTN.push_back(new BTN_EVENT(this, "HelpBeatmap", _question, 560, 735, 70, 70, 2));
    _Key_Listener = new BTN_EVENT(this);
    _Key_Listener->setFocus();
    _Key_Listener->grabKeyboard();
}

UI::~UI()
{
}
