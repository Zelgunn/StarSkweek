#include "player.h"

Player::Player()
{
    m_position.x = 0;
    m_position.y = 0;
    m_appearance = QImage(QApplication::applicationDirPath().append("/images/player.png"));
    m_speed = 0.005;
}

