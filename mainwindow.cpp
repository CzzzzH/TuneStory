#include "mainwindow.h"
#include "ui_contral.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle(tr("TuneStory"));
    setWindowIcon(QIcon(":/image/icon.png"));
    setGeometry(0,0,1920,1080);
    _MousePicture = new QPixmap(":/image/Mouse.png");
    _Mouse = new QCursor(*_MousePicture);
    this->setMouseTracking(true);
    this->setCursor(*_Mouse);
    show();
}

MainWindow::~MainWindow()
{
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->setCursor(*_Mouse);
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    __UI->Setting("Save");
}
