#include "playerinfo.h"

PlayerInfo::PlayerInfo()
    : m_characterSelected(0), m_ready(false)
{

}

int PlayerInfo::characterSelected() const
{
    return m_characterSelected;
}

void PlayerInfo::setCharacterSelected(int characterSelected)
{
    m_characterSelected = characterSelected;
}

QString PlayerInfo::nickname() const
{
    return m_nickname;
}

void PlayerInfo::setNickname(const QString &nickname)
{
    m_nickname = nickname;
}

QHostAddress PlayerInfo::address() const
{
    return m_address;
}

void PlayerInfo::setAddress(const QHostAddress &address)
{
    m_address = address;
}
bool PlayerInfo::ready() const
{
    return m_ready;
}

void PlayerInfo::setReady(bool ready)
{
    m_ready = ready;
}

