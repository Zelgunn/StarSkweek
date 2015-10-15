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
    QList<Projectile *> projectiles() const;

    bool movePlayer(int playerNumber, GameObject::Directions direction);
    bool movePlayer1(GameObject::Directions direction);
    bool movePlayer2(GameObject::Directions direction);

    void setPlayerDirection(int playerId, GameObject::Directions direction);

    bool playerFires(int playerId);

    double playerTileRatio() const;

    void nextFrame();

private:
    Grid m_grid;
    Player m_player;
    Player m_player2;
    int m_myPlayer;
    QList<Projectile *> m_projectiles;
};

#endif // LEVEL_H
