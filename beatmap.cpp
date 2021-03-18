#include "beatmap.h"

class UI;

Beatmap::Beatmap()
{
    for (qint32 k = 0; k < 4; ++k)
        _NoteStyle[k].push_back("0");
}

Beatmap::~Beatmap()
{
}
