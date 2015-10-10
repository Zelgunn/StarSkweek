#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include "arduinohandler.h"

class SoundPlayer
{
public:
    SoundPlayer();

private:
    ArduinoHandler *m_arduinohandler;
};

#endif // SOUNDPLAYER_H
