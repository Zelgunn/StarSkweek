#ifndef PLAYERINFO_H
#define PLAYERINFO_H

#include <QString>
#include <QHostAddress>

class PlayerInfo
{
public:
    PlayerInfo();

    int characterSelected() const;
    void setCharacterSelected(int characterSelected);

    QString nickname() const;
    void setNickname(const QString &nickname);

    QHostAddress address() const;
    void setAddress(const QHostAddress &address);

    bool ready() const;
    void setReady(bool ready);

private:
    int m_characterSelected;
    QString m_nickname;
    QHostAddress m_address;
    bool m_ready;
};

#endif // PLAYERINFO_H
