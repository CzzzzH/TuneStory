#include "btn_event.h"
#include "data.h"
#include "inputbox.h"
#include "sqlconnect.h"
#include "ui_contral.h"

void BTN_EVENT::setAvailable(bool _Signal)
{
    _Available = _Signal;
    if (_Signal) setAlpha(0.8);
    else setAlpha(0.25);
}

BTN_EVENT::BTN_EVENT(UI *__UI) // initialization for keylistener
{
    _UI = __UI;
}

// initialization for buttons

BTN_EVENT::BTN_EVENT(UI *__UI, QString __Type, QPixmap *__Picture, qint32 _x, qint32 _y, qint32 _w, qint32 _h, qint32 __Flag)
{
    _UI = __UI;
    _Type = __Type;
    _Flag = __Flag;
    this->setParent(_UI->_Screen);
    _Transparent.setParent(this);
    setGraphicsEffect(&_Transparent);
    _Available = true;
    setScaledContents(true);
    setPixmap(*__Picture);
    setGeometry(_x, _y, _w, _h);
    setAlpha(1);
    hide();
}

BTN_EVENT::~BTN_EVENT()
{
}

void BTN_EVENT::keyPressEvent(QKeyEvent *event)
{
    if (_UI->_Transfer->_Timer.isActive()) return;
    if (_UI->_SwitchTimer.isActive()) return;
    if (_UI->_LoadingState == "Connecting") return;
    if (_UI->_ifChangeKey) // grab the keyboard when changing the keys of hitting
    {
        if (event->key() == Qt::Key_Escape) // Some keys are not allowed to use
        {
            _UI->_SettingText[_UI->_ifChangeKey]->setStyleSheet("color:black;");
            _UI->_ifChangeKey = 0;
            return;
        }
        if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) return;
        if (event->key() == Qt::Key_Control || event->key() == Qt::Key_Shift) return;
        if (event->key() == Qt::Key_Alt) return;
        _UI->_SettingChar[_UI->_ifChangeKey - 4] = event->key();
        for (qint32 k = 0; k < 5; ++k)
        {
            if (k == _UI->_ifChangeKey - 4) continue;
            if (_UI->_SettingChar[k] == _UI->_SettingChar[_UI->_ifChangeKey - 4])
            {
                _UI->_SettingChar[k] = '\0';
                _UI->_SettingText[k + 4]->changeText("\0");
            }
        }
        _UI->_SettingText[_UI->_ifChangeKey]->setStyleSheet("color:black;");
        _UI->_SettingText[_UI->_ifChangeKey]->changeText(Data::convert(_UI->_SettingChar[_UI->_ifChangeKey - 4]));
        _UI->_ifChangeKey = 0;
        return;
    }
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        if (_UI->_CurrentScene == "MainTitle")
        {
            if (_UI->_ErrorNum == 1) // judge whether there is an error
            {
                new Item(_UI, "HintText", "读取谱面文件失败！请检查游戏完整性"
                    , "white", Data::font(1, 30, 20), 1600, 35, -1, true);
                return;
            }
            _UI->Transfer("MainTitle", "SongMenu");
            _UI->_Enter->play();
        }
        if (_UI->_CurrentScene == "Register")
        {
            _UI->Loading("Connecting");
            QtConcurrent::run(SqlConnect::CreateAccount);

        }
        if (_UI->_CurrentScene == "Login")
        {
            _UI->Loading("Connecting");
            QtConcurrent::run(SqlConnect::Login);
        }
        return;
    }
    if (event->modifiers() == Qt::ControlModifier)
    {
        if (event->key() == Qt::Key_F)
        {
            _UI->_Screen->_FullScreenFlag %2 == 0 ? _UI->_Screen->showNormal() : _UI->_Screen->showFullScreen();
            _UI->_Screen->_FullScreenFlag ++;
            return;
        }
        if (_UI->_CurrentScene == "Setting") return;
        if (event->key() == Qt::Key_Minus || event->key() == Qt::Key_Equal)
        {
             if (event->key() == Qt::Key_Equal) _UI->_HitOffset ++;
             else _UI->_HitOffset --;
             new Item(_UI, "HintText", "您当前的打击延迟(HitOffset)是: " + QString::number(_UI->_HitOffset)
                + "ms", "white", Data::font(1, 30, 20), 1600, 35, -1, true);
             return;
        }
        if (event->key() == Qt::Key_0 || event->key() == Qt::Key_9)
        {
             if (event->key() == Qt::Key_0) _UI->_MusicOffset ++;
             else _UI->_MusicOffset --;
             new Item(_UI, "HintText", "您当前的音乐延迟(MusicOffset)是: " + QString::number(_UI->_MusicOffset)
                + "ms", "white", Data::font(1, 30, 20), 1600, 35, -1, true);
             return;
        } 
        if (event->key() == Qt::Key_S)
        {
            _UI->_ifHitSound ? _UI->_ifHitSound = false : _UI->_ifHitSound = true;
            _UI->_ifHitSound ? _UI->_SettingChar[5] = 'T' : _UI->_SettingChar[5] = 'F';
            QString _State = _UI->_ifHitSound ? "开" : "关";
            new Item(_UI, "HintText", "打击音效: " + _State, "white", Data::font(1, 30, 20), 1600, 35, -1, true);
            return;
        }
    }
    for (qint32 k = 0; k < 5; ++k)
    {
        qint32 _ColourCount = 2;
        if (event->key() != _UI->_SettingChar[k]) continue;
        if (k <= 1) _ColourCount = 0;
        if (k >= 3) _ColourCount = 1;
        if (_UI->_NoteQueue[_ColourCount]->isEmpty()) return;
        hit(_ColourCount, _UI->_NoteQueue[_ColourCount]->head());
        return;
    }
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)
    {
        if (_UI->_CurrentScene != "SongMenu" || _UI->_isPause) return;
        if (event->key() == Qt::Key_Down) _UI->_CurrentSong ++;
        else _UI->_CurrentSong += _UI->_SongList.size() - 1;
        _UI->_CurrentSong %= _UI->_SongList.size();
        if (event->key() == Qt::Key_Down) _UI->Get_Current_Song("Down");
        else _UI->Get_Current_Song("Up");
        _UI->_SwitchSong->play();
        return;
    }
    if (event->key() == Qt::Key_R)
    {
       if (_UI->_CurrentScene != "Result") return;
       _UI->Loading("Connecting");
       QtConcurrent::run(SqlConnect::UploadScore);
    }
    if  (event->key() == Qt::Key_Escape)
    {
        if (_UI->_CurrentScene == "MainTitle")
        {
             _UI->Setting("Save");
             QApplication::quit();
        }
        if (_UI->_CurrentScene == "Result")
        {
            _UI->Transfer("Result", "SongMenu");
            return;
        }
        if (_UI->_CurrentScene == "PlaySong")
        {
            if (!_UI->_isPause)
            {
                _UI->_CurrentSongLength = _UI->_SongTimer.remainingTime();
                _UI->_NoteTimer.stop();
                _UI->_JudgeTimer.stop();
                _UI->_SongTimer.stop();
                _UI->_BGM->pause();
                _UI->_Gray->setPixmap(*_UI->_Pause);
                _UI->_Gray->show_with_raise();
                for (auto it = _UI->_PauseBTN.begin(); it != _UI->_PauseBTN.end(); ++it)
                {
                    if ((*it)->_Type == "Back_C") (*it)->move(320, 400);
                    if ((*it)->_Type != "Quit_C") (*it)->show_with_raise();
                }
                _UI->_isPause = true;
            }
            else
            {
                _UI->_SongTimer.start(_UI->_CurrentSongLength);
                _UI->_NoteTimer.start(16);
                _UI->_JudgeTimer.start(5);
                _UI->_BGM->play();
                _UI->_Gray->hide();
                for (auto it = _UI->_PauseBTN.begin(); it != _UI->_PauseBTN.end(); ++it)
                    (*it)->hide();
                _UI->_isPause = false;
            }
            return;
        }
        if (_UI->_CurrentScene == "SongMenu")
        {
            if (!_UI->_isPause)
            {
                _UI->Get_Song_Info("Hide");
                _UI->_Gray->setPixmap(*_UI->_IfQuit);
                _UI->_Gray->show_with_raise();
                for (auto it = _UI->_PauseBTN.begin(); it != _UI->_PauseBTN.end(); ++it)
                {
                    if ((*it)->_Type == "Back_C") (*it)->move(570, 400);
                    if ((*it)->_Type == "Back_C" || (*it)->_Type == "Quit_C")
                        (*it)->show_with_raise();
                }
                _UI->_isPause = true;
            }
            else
            {
                _UI->_Gray->hide();
                for (auto it = _UI->_PauseBTN.begin(); it != _UI->_PauseBTN.end(); ++it)
                    (*it)->hide();
                _UI->_isPause = false;
            }
            return;
        }
        if (_UI->_CurrentScene == "Setting")
        {
            _UI->_CurrentScene = "SongMenu";
            _UI->_Gray->hide();
            if (_UI->_iflogin) _UI->_PlayerName = _UI->_NameBoard->text();
            _UI->_NameBoard->hide();          
            for (auto it = _UI->_SettingBTN.begin(); it != _UI->_SettingBTN.end(); ++it)
                (*it)->hide();
            for (auto it = _UI->_SettingText.begin(); it != _UI->_SettingText.end(); ++it)
                (*it)->hide();
            for (auto it = _UI->_FunctionBTN.begin(); it != _UI->_FunctionBTN.end(); ++it)
                (*it)->hide();
            for (auto it = _UI->_PlayerInfo.begin(); it != _UI->_PlayerInfo.end(); ++it)
                (*it)->hide();
            return;
        }
        if (_UI->_CurrentScene == "Login" || _UI->_CurrentScene == "Register")
        {
            _UI->_GrayPlus->hide();
            _UI->_NameBoard->hide();
            _UI->_PasswordBoard->hide();
            _UI->_Key_Listener->grabKeyboard();
            _UI->_Screen->setFocus();
            _UI->_CurrentScene = "Setting";
            _UI->RefreshAccountState();
            for (auto it = _UI->_FunctionBTN.begin(); it != _UI->_FunctionBTN.end(); ++it) // Load the function buttons;
            {
                if ((*it)->_Type == "Yes" || (*it)->_Type == "Cancel")
                    (*it)->hide();
            }
        }
        if (_UI->_CurrentScene == "Help")
        {
            _UI->_CurrentScene = "SongMenu";
            _UI->_Gray->hide();
            for (auto it = _UI->_HelpBTN.begin(); it != _UI->_HelpBTN.end(); ++it)
                (*it)->hide();
            return;
        }
    }
}

void BTN_EVENT::enterEvent(QEvent *event) // events for mouse's over the button
{
    if (!_Available) return;
    if (_UI->_ifChangeKey) return;
    if (_UI->_SwitchTimer.isActive()) return;
    if (_UI->_SongMenuBTN.count(this))
    {
        setAlpha(0.95);
        if (_Flag >= 0)
        {
            _UI->Get_Song_Info(_Type);
            _UI->_MouseInfo = _Type;
        }
    }
    if (_UI->_SettingBTN.count(this) && _Flag >= 0)
    {
        if (_Flag < 2) setPixmap(*_UI->_RightArrow_L);
        else if (_Flag < 4) setPixmap(*_UI->_LeftArrow_L);
        else setPixmap(*_UI->_Settingbtn_L);
    }
    if (_Type == "PauseR" || _UI->_PauseBTN.count(this))
        setPixmap(*_UI->_PausePic[_Flag + 1]);  // Lighten the button
    if (_UI->_FunctionBTN.count(this))
        setPixmap(*_UI->_FunctionPic[_Flag + 1]);  // Lighten the button
    if (_UI->_HelpBTN.count(this))
        _UI->_HelpAnswer[_Flag]->show_with_raise();
}

void BTN_EVENT::leaveEvent(QEvent *event) // events for mouse's leave the button
{
    if (!_Available) return;
    if (_UI->_SwitchTimer.isActive()) return;
    if (_UI->_SongMenuBTN.count(this))
    {
        setAlpha(0.8);
        if (_Flag >= 0)
        {
            _UI->_MouseInfo = "NULL";
            _UI->Get_Song_Info("Hide");
        }
    }
    if (_UI->_SettingBTN.count(this) && _Flag >= 0)
    {
        if (_Flag < 2) setPixmap(*_UI->_RightArrow);
        else if (_Flag < 4) setPixmap(*_UI->_LeftArrow);
        else setPixmap(*_UI->_Settingbtn);
    }
    if (_Type == "PauseR" || _UI->_PauseBTN.count(this))
        setPixmap(*_UI->_PausePic[_Flag]); // Darken the button
    if (_UI->_FunctionBTN.count(this))
        setPixmap(*_UI->_FunctionPic[_Flag]);  // Darken the button
    if (_UI->_HelpBTN.count(this))
        _UI->_HelpAnswer[_Flag]->hide();
}

void BTN_EVENT::mousePressEvent(QMouseEvent *event)
{
    if (!_Available) return;
    if (_UI->_SwitchTimer.isActive()) return;
    if (event->button() != Qt::LeftButton) return;
    if (_UI->_ifChangeKey)
    {
        new Item(_UI, "HintText","请按下想要修改的键（部分功能键无法被使用），或按Esc取消", "white", Data::font(1, 30, 20), 1500, 35, -1, true);
        return;
    }
    _UI->_Click->play();
    if (_Type == "Back_C" || _Type == "PauseR")
        QCoreApplication::sendEvent(_UI->_Key_Listener, _UI->_EscapeEvent);
    if (_Type == "Restart_C")
    {
        QCoreApplication::sendEvent(_UI->_Key_Listener, _UI->_EscapeEvent);
        _UI->PlaySong("Hide");
        _UI->PlaySong("Show");
    }
    if (_Type == "Menu_C")
    {
        QCoreApplication::sendEvent(_UI->_Key_Listener, _UI->_EscapeEvent);
        _UI->_BGM->stop();
        _UI->_NoteTimer.stop();
        _UI->_JudgeTimer.stop();
        _UI->CleanStage();
        _UI->Transfer("PlaySong", "SongMenu");
        return;
    }
    if (_Type == "Back")
    {
        if (_UI->_CurrentScene == "SongMenu")
            _UI->Transfer("SongMenu", "MainTitle");
        if (_UI->_CurrentScene == "Result")
            QCoreApplication::sendEvent(_UI->_Key_Listener, _UI->_EscapeEvent);
        if (_UI->_CurrentScene == "Setting" || _UI->_CurrentScene == "Help")
            QCoreApplication::sendEvent(_UI->_Key_Listener, _UI->_EscapeEvent);
        return;
    }
    if (_Type == "Restart")
        _UI->Transfer("Result", "PlaySong");
    if (_Type == "Setting")
    {
        _UI->_Gray->setPixmap(*_UI->_Settings);
        _UI->_Gray->show_with_raise();
        _UI->_SongMenuBTN[0]->raise();
        _UI->_CurrentScene = "Setting";
        for (qint32 k = 0; k < _UI->_SettingBTN.size(); ++k) // Load the setting text
        {
            if (k == 0) _UI->_SettingText[k]->changeText(QString::number(qint32(_UI->_HitOffset)));
            if (k == 2) _UI->_SettingText[k]->changeText(QString::number(qint32(_UI->_MusicOffset)));
            if (k >= 4 && k <= 8) _UI->_SettingText[k]->changeText(Data::convert(_UI->_SettingChar[k - 4]));
            if (k > 8)  _UI->_SettingText[k]->changeText(Data::convert(_UI->_SettingChar[k - 4], 1));
            _UI->_SettingBTN[k]->show_with_raise();
            _UI->_SettingText[k]->show_with_raise();
        }
        _UI->RefreshAccountState();
    }
    if (_Type == "Help")
    {
        _UI->_CurrentScene = "Help";
        _UI->_Gray->setPixmap(*_UI->_HelpBoard);
        _UI->_Gray->show_with_raise();
        _UI->_SongMenuBTN[0]->raise();
        for (auto it = _UI->_HelpBTN.begin(); it != _UI->_HelpBTN.end(); ++it)
            (*it)->show_with_raise();
    }
    if (_UI->_SongMenuBTN.count(this) && _Flag >= 0)
    {
        _UI->_Difficulty = _Flag;
        _UI->Transfer("SongMenu", "PlaySong");
    }
    if (_UI->_SettingBTN.count(this))
    {
        if (_Flag >= 0 && _Flag <= 3)
        {
            if (_Type == "H_P") _UI->_HitOffset ++;
            if (_Type == "M_P") _UI->_MusicOffset ++;
            if (_Type == "H_M") _UI->_HitOffset --;
            if (_Type == "M_M") _UI->_MusicOffset --;
            _UI->_SettingText[0]->changeText(QString::number(qint32(_UI->_HitOffset)));
            _UI->_SettingText[2]->changeText(QString::number(qint32(_UI->_MusicOffset)));
            return;
        }
        if (_Flag >=4 && _Flag <= 8)
        {
            _UI->_ifChangeKey = _Flag;
            _UI->_SettingText[_Flag]->setStyleSheet("color:red;");
            new Item(_UI, "HintText","请按下想要修改的键（部分功能键无法被使用），或按Esc取消", "white", Data::font(1, 30, 35), 1500, 35, -1, true);
        }
        if (_Flag == 9)
        {
            _UI->_ifHitSound ? _UI->_ifHitSound = false : _UI->_ifHitSound = true;
            _UI->_ifHitSound ? _UI->_SettingChar[_Flag - 4] = 'T' : _UI->_SettingChar[_Flag - 4] = 'F';
            _UI->_SettingText[_Flag]->changeText(Data::convert(_UI->_SettingChar[_Flag - 4], 1));
        }
        if (_Flag == 10)
        {
            _UI->_ifBeginner ? _UI->_ifBeginner = false : _UI->_ifBeginner = true;
            _UI->_ifBeginner ? _UI->_SettingChar[_Flag - 4] = 'T' : _UI->_SettingChar[_Flag - 4] = 'F';
            _UI->_SettingText[_Flag]->changeText(Data::convert(_UI->_SettingChar[_Flag - 4], 1));
        }
        return;
    }
    if (_UI->_FunctionBTN.count(this))
    {
        if (_Type == "Logout")
        {
            _UI->_iflogin = false;
            _UI->_PlayerName = "Guest";
            _UI->RefreshAccountState();
            return;
        }
        if (_Flag >= 4)
        {
            _UI->_GrayPlus->show_with_raise();
            _UI->_NameBoard->show_with_raise();
            _UI->_NameBoard->clear();
            _UI->_PasswordBoard->show_with_raise();
            _UI->_PasswordBoard->clear();
            if (_Type == "Login")
            {
                _UI->_CurrentScene = "Login";
                _UI->_GrayPlus->setPixmap(*_UI->_LoginScreen);
            }
            if (_Type == "Register")
            {
                _UI->_CurrentScene = "Register";
                _UI->_GrayPlus->setPixmap(*_UI->_RegisterScreen);
            }
            for (auto it = _UI->_FunctionBTN.begin(); it != _UI->_FunctionBTN.end(); ++it)
                if ((*it)->_Type == "Yes" || (*it)->_Type == "Cancel")
                    (*it)->show_with_raise();
        }
        if (_Type == "Cancel")
            QCoreApplication::sendEvent(_UI->_Key_Listener, _UI->_EscapeEvent);
        if (_Type == "Yes")
            QCoreApplication::sendEvent(_UI->_Key_Listener, _UI->_ReturnEvent);
        return;
    }
    if (_Type == "Quit" || _Type == "Quit_C")
    {
        if (_UI->_CurrentScene == "SongMenu")
        {
            _UI->Setting("Save");
            QApplication::quit();
        }
    }
}
