#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include "network/arduinohandler.h"

class SoundPlayer
{
public:
    SoundPlayer();

private:
    ArduinoHandler *m_arduinohandler;
};

#endif // SOUNDPLAYER_H
