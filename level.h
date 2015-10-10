#ifndef LEVEL_H
#define LEVEL_H

#include "grid.h"
#include "player.h"
#include "monster.h"

class Level
{
public:
    Level();
    Level(QDomElement elem);

    const Grid *grid() const;
    const Player *player() const;

    void movePlayer(GameObject::Directions direction);

private:
    Grid m_grid;
    Player m_player;
};

#endif // LEVEL_H
