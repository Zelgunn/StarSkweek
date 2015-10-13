#ifndef LEVEL_H
#define LEVEL_H

#include "grid.h"
#include "player.h"
#include "monster.h"
#include "tile.h"

class Level
{
public:
    Level();

    void setMyPlayer(int playerNumber);
    const Grid *grid() const;
    const Player *player() const;
    const Player *player2() const;

    bool movePlayer(int playerNumber, GameObject::Directions direction);
    bool movePlayer1(GameObject::Directions direction);
    bool movePlayer2(GameObject::Directions direction);

    double playerTileRatio() const;

private:
    Grid m_grid;
    Player m_player;
    Player m_player2;
    int m_myPlayer;
};

#endif // LEVEL_H
