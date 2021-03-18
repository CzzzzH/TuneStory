#include "mainwindow.h"
#include "ui_contral.h"
#include "btn_event.h"
#include "beatmap.h"
#include "note.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::addLibraryPath("./plugins");
    QApplication app(argc, argv);
    MainWindow _MainWindow;
    UI *_UI = new UI(&_MainWindow);
    _MainWindow.__UI = _UI;
    _UI->Setting("Load");
    _UI->Load_Beatmap();
    _UI->Load_Sound();
    _UI->Initialize();
    _UI->MainTitle("Show"); 
    return app.exec();
}

// Qt has its intelligent method in managing the memory.
// So it is not necessary to free most variables in the program.
