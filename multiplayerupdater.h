#ifndef MULTIPLAYERUPDATER_H
#define MULTIPLAYERUPDATER_H

#include <QStringList>

class MultiplayerUpdater
{
public:
    MultiplayerUpdater();

    void addUpdate(const QString &update);

private:
    QStringList m_updates;
};

#endif // MULTIPLAYERUPDATER_H
