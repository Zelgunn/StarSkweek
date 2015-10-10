#include "multiplayerupdater.h"

MultiplayerUpdater::MultiplayerUpdater()
{

}

void MultiplayerUpdater::addUpdate(const QString &update)
{
    m_updates.append(update);
}

