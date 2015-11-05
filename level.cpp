#include "level.h"

Level::Level(const QDomElement &element, const QList<const Player *> *prototypes, const QList<PlayerInfo *> &playersInfos) :
    m_prototypes(prototypes), m_myPlayer(0)
{
    PlayerInfo *playerInfo;
    for(int i=0; i<playersInfos.size(); i++)
    {
        playerInfo = playersInfos.at(i);
        m_players.append(prototypes->at(playerInfo->characterSelected())->clone());
    }

    m_name = element.attribute("name");
    m_tiles = new Tile[Tile::TypeCount];
    int tileIndex;
    QString filename;
    QDomElement elem;
    QDomNode node = element.firstChild();

    while(!node.isNull())
    {
        elem = node.toElement();

        if(elem.tagName() == "Grid")
        {
            m_grid = new Grid(elem);
        }

        if(elem.tagName() == "TileSize")
        {
            m_tileSize.setHeight(elem.attribute("height").toInt());
            m_tileSize.setWidth(elem.attribute("width").toInt());
        }

        if(elem.tagName() == "Tile")
        {
            tileIndex = elem.attribute("type").toLatin1().at(0) - 'a';
            filename = QApplication::applicationDirPath() + elem.attribute("filename");
            m_tiles[tileIndex] = Tile(filename, m_tileSize, (Tile::TileType)tileIndex);
        }

        if(elem.tagName() == "Projectiles")
        {
            m_projectiles = new ProjectileList(elem);
        }

        if(elem.tagName() == "Weapons")
        {
            m_weapons = WeaponList(elem);
        }

        node = node.nextSibling();
    }

    Player *player;
    player = m_players.at(0);
    player->setSpeed(qPow(m_tileSize.width() * m_tileSize.height(), 0.375));
    player->setFaction(0);
    player->setWeapon(m_weapons.at(0));
    m_projectiles->appendCollision(player);

    for(int i=1; i<playersInfos.size(); i++)
    {
        player = m_players.at(i);

        player->setFaction(i);
        player->setWeapon(m_weapons.at(0));
        m_projectiles->appendCollision(player);
    }

    QObject::connect(m_projectiles, SIGNAL(hitPlayer(GameObject*,int)), this, SLOT(onPlayerHit(GameObject*,int)));
}

void Level::setMyPlayer(int playerNumber)
{
    Player *player = m_players.at(0);
    switch(playerNumber)
    {
    case 0:
        player->setPosition(width()/2, height()/4);
        break;
    default:
        player->setPosition(width()/2, height()*3/4 - 5);
        break;
    }
}

const Grid *Level::grid() const
{
    return m_grid;
}

const Player *Level::player(int index) const
{
    return m_players.at(index);
}

QList<Player *> Level::players() const
{
    return m_players;
}

const ProjectileList *Level::projectiles() const
{
    return m_projectiles;
}

QSize Level::tileSize() const
{
    return m_tileSize;
}

Tile *Level::tiles() const
{
    return m_tiles;
}

int Level::width() const
{
    return m_tileSize.width() * m_grid->width();
}

int Level::height() const
{
    return m_tileSize.height() * m_grid->height();
}

bool Level::movePlayer(int playerId, GameObject::Directions direction)
{
    Player *player = m_players.at(playerId);

    if(player->dead())
        return false;

    QPoint displacement = GameObject::displacement(direction, player->speed());
    displacement += player->position();

    if((displacement.x() < 0)         ||
       (displacement.y() < 0)         ||
       (displacement.x() >= width())  ||
       (displacement.y() >= height())) return false;

    return setPlayerPosition(playerId, displacement.x(), displacement.y(), direction);
}

void Level::arrowTileMove(int playerNumber)
{
    Player *player = m_players.at(playerNumber);
    if(player->dead() || player->invulnerable()) return;

    bool isMoveValid = true;
    int w = m_tileSize.width(), h = m_tileSize.height();
    QPoint nextPosition = player->position();
    uint x = nextPosition.x() / w;
    uint y = nextPosition.y() / h;
    Tile::TileType tileType = m_grid->tileAt(x,y);
    qreal playerSpeed = player->speed()/20 + 2;

    if(tileType == Tile::ArrowTileDown)
        nextPosition.ry() += (int)playerSpeed;
    if(tileType == Tile::ArrowTileLeft)
        nextPosition.rx() -= (int)playerSpeed;
    if(tileType == Tile::ArrowTileRight)
        nextPosition.rx() += (int)playerSpeed;
    if(tileType == Tile::ArrowTileUp)
        nextPosition.ry() -= (int)playerSpeed;

    if(nextPosition != player->position())
        isMoveValid = false;

    if(tileType == Tile::Void)
    {
        return;
    }

    x = nextPosition.x() / w;
    y = nextPosition.y() / h;
    if(m_grid->tileAt(x,y) == Tile::Void)
    {
        player->takeDamage(99999);
        isMoveValid = false;
    }

    player->setPosition(nextPosition, isMoveValid);
}

bool Level::setPlayerPosition(int playerId, int x, int y, GameObject::Directions direction)
{
    Player *player = m_players.at(playerId);
    QList<Player *> otherPlayers;
    bool isMoveValid = true;

    for(int i=0; i<m_players.size(); i++)
    {
        if(i != playerId)
        {
            otherPlayers.append(m_players.at(i));
        }
    }

    int w = m_tileSize.width(), h = m_tileSize.height();

    uint tx = x / w;
    uint ty = y / h;
    Tile::TileType nextTileType = m_grid->tileAt(tx, ty);

    // Case de type vide : La mort !
    if(m_grid->tileAt(player->position().x() / w, player->position().y() / h) == Tile::Void)
    {
        isMoveValid = false;
        if(nextTileType == Tile::Void)
        {
            player->takeDamage(99999);
            return false;
        }
    }

    // L'étage
    player->setUpstairs(nextTileType == Tile::UpstairsTile);

    // Case non valide pour la résurrection
    switch(nextTileType)
    {
    case Tile::ArrowTileDown:
    case Tile::ArrowTileLeft:
    case Tile::ArrowTileRight:
    case Tile::ArrowTileUp:
    case Tile::Void:
        isMoveValid = false;
        break;
    default:
        break;
    }

    player->setPosition(x, y, isMoveValid);
    player->setDirection(direction);

    // Changement de la couleur de la case.
    if(otherPlayers.size() == 0)
    {
        if(m_grid->tileAt(tx, ty) == Tile::Player2Tile)
        {
            m_grid->setTileAt(tx, ty, Tile::Player1Tile);
        }
    }
    else
    {
        foreach(Player *otherPlayer, otherPlayers)
        {
            if(m_grid->tileAt(tx, ty) == otherPlayer->tileType())
            {
                m_grid->setTileAt(tx, ty, player->tileType());
                break;
            }
        }
    }

    return false;
}

bool Level::playerFires(int playerId)
{
    Player *player = m_players.at(playerId);
    int type = player->fire();

    if(type >= 0)
    {
        m_projectiles->append(type, playerId, player->previousDirection(), player->position());

        return true;
    }

    return false;
}

double Level::playerTileRatio(int player) const
{
    double res = 0, count = 0;
    QList<Tile::TileType> playersTiles;
    // Pour plus que 2 joueurs
//    for(int i=0; i<m_players.size(); i++)
//    {
//        playersTiles.append(m_players.at(i)->tileType());
//    }
    // Sinon pour 1-2 joueurs
    playersTiles.append(Tile::Player1Tile);
    playersTiles.append(Tile::Player2Tile);

    Tile::TileType tmpTile, playerTile = m_players.at(player)->tileType();

    for(uint i=0; i<m_grid->width(); i++)
    {
        for(uint j=0; j<m_grid->height(); j++)
        {
            tmpTile = m_grid->tileAt(i,j);
            if(playersTiles.contains(tmpTile))
                count ++;

            if(tmpTile == playerTile)
                res++;
        }
    }

    if(count == 0) return 0;
    return res/count;
}

void Level::nextFrame()
{
    m_projectiles->moveProjectiles();

    Player *player;
    for(int i=0; i<m_players.size(); i++)
    {
        player = m_players.at(i);
        arrowTileMove(i);
        player->updateLifeAnim();
    }
}

void Level::onPlayerHit(GameObject *player, int type)
{
    Player *p = (Player*) player;
    p->takeDamage(m_projectiles->prototype(type).damage());
}
