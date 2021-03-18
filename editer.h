#ifndef EDITER_H
#define EDITER_H

#include <QObject>
#include <QDir>
#include <QFile>

class UI;

class Editer : public QObject
{
    Q_OBJECT

public:

    UI *_UI;
    Editer(UI *__UI);
    ~Editer();
};

#endif // EDITER_H
