#ifndef UI_CONTRAL_H
#define UI_CONTRAL_H

#include <QPixmap>
#include <QMediaPlayer>
#include <QObject>
#include <QMovie>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QFont>
#include <QFontDatabase>
#include <QtDebug>
#include <QtMath>
#include <QLabel>
#include <QQueue>
#include <QLineEdit>
#include <QSoundEffect>
#include <algorithm>
#include <QtMath>


class Item;
class BTN_EVENT;
class Beatmap;
class InputBox;
class Note;
class MainWindow;
class ScoreBoard;

class UI : public QObject
{
    Q_OBJECT

public:

    struct PlayInfo
    {
        QString _Name[11];
        QString _Score[11];
        QString _Date[11];
    };
    qint32 _CurrentSong;
    qint32 _Num;
    qint32 _Difficulty;
    qint32 _RainbowHit;
    qint32 _NormalHit;
    qint32 _Miss;
    qint32 _Combo;
    qint32 _MaxCombo;
    qint32 _HitOffset;
    qint32 _MusicOffset;
    qint32 _Hintnum;
    qint32 _CurrentSongTime;
    qint32 _CurrentSongLength;
    qint32 _ifChangeKey;
    qint32 _CenterBoardNum;
    qint32 _Rank;
    qint32 _ErrorNum;
    qreal _TotalScore;
    qreal _ComboScore;
    qreal _NoteScore;
    qreal _AvgNote;
    qreal _ComboLow;
    qreal _ComboHigh;
    qreal _SwitchTime;
    bool _isPause;
    bool _ifHitSound;
    bool _ifBeginner;
    bool _iflogin;
    bool _ifUpdate;
    MainWindow *_Screen;
    QKeyEvent *_EscapeEvent;
    QKeyEvent *_ReturnEvent;
    QString _LoadingState;
    QString _CurrentScene;
    QString _SettingChar;
    QString _PlayerName;
    QString _MouseInfo;
    QPixmap *_MainTitle;
    QPixmap *_Black;
    QPixmap *_back;
    QPixmap *_restart;
    QPixmap *_quit;
    QPixmap *_editer;
    QPixmap *_setting;
    QPixmap *_easy;
    QPixmap *_normal;
    QPixmap *_hard;
    QPixmap *_extreme;
    QPixmap *_help;
    QPixmap *_EasyB;
    QPixmap *_NormalB;
    QPixmap *_HardB;
    QPixmap *_ExtremeB;
    QPixmap *_BeginnerMode;
    QPixmap *_PauseR;
    QPixmap *_PauseR_L;
    QPixmap *_Red1;
    QPixmap *_Red2;
    QPixmap *_Red3;
    QPixmap *_Red1_L;
    QPixmap *_Red2_L;
    QPixmap *_Red3_L;
    QPixmap *_Blue1;
    QPixmap *_Blue2;
    QPixmap *_Blue3;
    QPixmap *_Blue1_L;
    QPixmap *_Blue2_L;
    QPixmap *_Blue3_L;
    QPixmap *_Green1;
    QPixmap *_Green2;
    QPixmap *_Green3;
    QPixmap *_Green1_L;
    QPixmap *_Green2_L;
    QPixmap *_Green3_L;
    QPixmap *_Rainbow1;
    QPixmap *_Rainbow2;
    QPixmap *_Rainbow3;
    QPixmap *_RainboWE;
    QPixmap *_NormalE;
    QPixmap *_MissE;
    QPixmap *_Back_C;
    QPixmap *_Back_CL;
    QPixmap *_Restart_C;
    QPixmap *_Restart_CL;
    QPixmap *_Menu_C;
    QPixmap *_Menu_CL;
    QPixmap *_Quit_C;
    QPixmap *_Quit_CL;
    QPixmap *_Pause;
    QPixmap *_IfQuit;
    QPixmap *_Settings;
    QPixmap *_LeftArrow;
    QPixmap *_LeftArrow_L;
    QPixmap *_RightArrow;
    QPixmap *_RightArrow_L;
    QPixmap *_Settingbtn;
    QPixmap *_Settingbtn_L;
    QPixmap *_LevelBoard;
    QPixmap *_LevelStar1;
    QPixmap *_LevelStar2;
    QPixmap *_LevelStar3;
    QPixmap *_LevelStar4;
    QPixmap *_HelpBoard;
    QPixmap *_question;
    QPixmap *_HelpOfKey;
    QPixmap *_HelpBeginner;
    QPixmap *_HelpBeatmap;
    QPixmap *_LoginScreen;
    QPixmap *_RegisterScreen;
    QPixmap *_Connecting;
    QPixmap *_LoadingL;
    Item *_LastHint;
    Item *_Author;
    Item *_BackStyle;
    Item *_BackGround;
    Item *_SongPicture;
    Item *_PressEnterUI;
    Item *_Transfer;
    Item *_SongNumTitle;
    Item *_Gray;
    Item *_GrayPlus;
    Item *_Brown;
    Item *_HighScoreName;
    Item *_HighScore;
    Item *_HighScoreRank;
    Item *_ConnectingScreen;
    Item *_LoadingSign;
    InputBox *_NameBoard;
    InputBox *_PasswordBoard;
    QTimer _SongTimer;
    QTimer _NoteTimer;
    QTimer _JudgeTimer;
    QTimer _SwitchTimer;
    QMovie *_PressEnter;
    QMediaPlayer *_BGM;
    QSoundEffect *_SwitchSong;
    QSoundEffect *_Click;
    QSoundEffect *_Enter;
    BTN_EVENT *_Key_Listener;
    BTN_EVENT *_BackBTN;
    BTN_EVENT *_RestartBTN;
    BTN_EVENT *_MenuBTN;
    BTN_EVENT *_QuitBTN;
    BTN_EVENT *_PauseRBTN;
    QVector<QPixmap*> _NotePic;
    QVector<QPixmap*> _PausePic;
    QVector<QPixmap*> _RankPic;
    QVector<QPixmap*> _NumPic;
    QVector<QPixmap*> _FunctionPic;
    QVector<Beatmap*> _SongList;
    QVector<BTN_EVENT*> _SongMenuBTN;
    QVector<BTN_EVENT*> _SettingBTN;
    QVector<BTN_EVENT*> _HelpBTN;
    QVector<BTN_EVENT*> _PauseBTN;
    QVector<BTN_EVENT*> _FunctionBTN;
    QVector<Item*> _SettingText;
    QVector<Item*> _PlayerInfo;
    QVector<Item*> _SongNameBoard;
    QVector<Item*> _PlayBoard;
    QVector<Item*> _SongInfo;
    QVector<Item*> _ScoreTextBoard;
    QVector<Item*> _StarBoard;
    QVector<Item*> _HelpAnswer;
    QVector<ScoreBoard*> _RankList;
    QVector<QQueue<Note*>*> _NoteQueue;
    QVector<PlayInfo>_RankInfo;
    UI(MainWindow *screen);
    ~UI();
    void MainTitle(QString _Command);
    void SongMenu(QString _Command);
    void PlaySong(QString _Command);
    void Result(QString _Command);
    void RefreshAccountState();
    void RefreshRankList(QString _Difficulty);
    void Transfer(QString _SceneA, QString _SceneB);
    void Transfer(QString _Scene);
    void SwitchScene(QString _Command, QString _Scene);
    void Get_Current_Song(QString _Command = "Still");
    void Get_Song_Info(QString _Command);
    void RefreshScore();
    void Load_Beatmap();
    void Load_Sound();
    void Loading(QString _Command);
    void Initialize();
    void Setting(QString _Command);
    void SetFont();
    void SetKey(QString _Command, qint32 _KeyNum, QChar _Key);

signals:
    void CleanStage();

};

#endif // UI_CONTRAL_H
