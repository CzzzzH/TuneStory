#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QKeyEvent>
#include <QtDebug>
#include <QIcon>

class UI;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    qint32 _FullScreenFlag = 0;
    QCursor *_Mouse;
    QPixmap *_MousePicture;
    UI *__UI;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void SaveSetting();

protected:

    void mouseMoveEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
