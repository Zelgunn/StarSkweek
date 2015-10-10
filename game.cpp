#include "game.h"

Game::Game() :
    m_lifes(0), m_score(0), m_timer(NULL)
{
    QDomElement tmp;
    m_levels.append(new Level(tmp));
}

const Level *Game::level(int i)
{
    Q_ASSERT(i >= 0);
    Q_ASSERT(i < m_levels.size());
    return m_levels.at(i);
}

void Game::movePlayer(GameObject::Directions direction)
{
    Level *level = m_levels.first();
    level->movePlayer(direction);
}
