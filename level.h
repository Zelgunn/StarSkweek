#ifndef LEVEL_H
#define LEVEL_H

#include <QDomElement>

#include "grid.h"
#include "player.h"
#include "monster.h"
#include "tile.h"

class Level
{
public:
    Level(const QDomElement &element, QList<Player> *characters);

    void setMyPlayer(int playerNumber);
    const Grid *grid() const;
    const Player *player() const;
    const Player *player2() const;
    QList<Projectile *> projectiles() const;
    QSize tileSize() const;
    Tile *tiles() const;

    bool movePlayer(int playerNumber, GameObject::Directions direction);
    bool movePlayer1(GameObject::Directions direction);
    bool movePlayer2(GameObject::Directions direction);

    void setPlayerDirection(int playerId, GameObject::Directions direction);

    bool playerFires(int playerId);

    double playerTileRatio() const;

    void nextFrame();


private:
    Grid *m_grid;
    Player **m_players;
    QList<Player> *m_characters;
    int m_myPlayer;
    QList<Projectile *> m_projectiles;
    QSize m_tileSize;
    Tile *m_tiles;
};

#endif // LEVEL_H
