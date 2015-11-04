#include "level.h"

Level::Level(const QDomElement &element, const QList<const Player *> *prototypes, const QList<PlayerInfo *> &playersInfos) :
    m_prototypes(prototypes), m_myPlayer(0)
{
    qDebug() << "Meow";
    foreach(PlayerInfo *playerInfo, playersInfos)
    {
        qDebug() << playerInfo->nickname() << " : " << playerInfo->characterSelected();
        m_players.append(m_prototypes->at(playerInfo->characterSelected())->clone());
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
    player->setPosition(width()/2, height()/4);
    player->setFaction(0);
    player->setWeapon(m_weapons.at(0));
    m_projectiles->appendCollision(player);

    for(int i=1; i<playersInfos.size(); i++)
    {
        player = m_players.at(i);

        player->setPosition(width()/2, height()*3/4 - 5);   // TMP
        player->setFaction(i);
        player->setWeapon(m_weapons.at(0));
        m_projectiles->appendCollision(player);
    }

    QObject::connect(m_projectiles, SIGNAL(hitPlayer(GameObject*,int)), this, SLOT(onPlayerHit(GameObject*,int)));
}

void Level::setMyPlayer(int playerNumber)
{
    if(playerNumber != 0)
    {
        Player *tmp = m_players.at(0);
        m_players.replace(0, m_players.at(playerNumber));
        m_players.replace(playerNumber, tmp);

        m_myPlayer = playerNumber;
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

    Point displacement = GameObject::displacement(direction, player->speed());
    displacement.x += player->position().x;
    displacement.y += player->position().y;

    int w = m_tileSize.width(), h = m_tileSize.height();

    if((displacement.x < 0)         ||
       (displacement.y < 0)         ||
       (displacement.x >= width())  ||
       (displacement.y >= height())) return false;

    uint x = displacement.x / w;
    uint y = displacement.y / h;

    if(m_grid->tileAt(x,y) == Tile::ArrowTileDown)
        displacement.y += 2;
    if(m_grid->tileAt(x,y) == Tile::ArrowTileLeft)
        displacement.x -= 2;
    if(m_grid->tileAt(x,y) == Tile::ArrowTileRight)
        displacement.x += 2;
    if(m_grid->tileAt(x,y) == Tile::ArrowTileUp)
        displacement.y -= 2;

    return setPlayerPosition(playerId, displacement.x, displacement.y, direction);
}

bool Level::setPlayerPosition(int playerId, int x, int y, GameObject::Directions direction)
{
    Player *player = m_players.at(playerId);
    QList<Player *> otherPlayers;
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

    // Case de type vide : La mort !
    if(m_grid->tileAt(tx, ty) == Tile::Void)
    {
        if(m_grid->tileAt(player->position().x / w, player->position().y / h) == Tile::Void)
            return false;
        player->takeDamage(999);
    }

    // L'étage
    player->setUpstairs(m_grid->tileAt(tx, ty) == Tile::UpstairsTile);

    player->setPosition(x, y);
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

double Level::playerTileRatio() const
{
    double res = 0, count = 0;
    int tmp;

    for(uint i=0; i<m_grid->width(); i++)
    {
        for(uint j=0; j<m_grid->height(); j++)
        {
            tmp = m_grid->tileAt(i,j);
            if(tmp == m_players.at(0)->tileType())
            {
                count ++;
                res ++;
            }
            else if(tmp == m_players.at(1)->tileType())
                count ++;
        }
    }

    return res/count;
}

void Level::nextFrame()
{
    m_projectiles->moveProjectiles();

    Player *player;
    for(int i=0; i<m_players.size(); i++)
    {
        player = m_players.at(i);
        //movePlayer(i, player->direction());
        player->updateLifeAnim();
    }
}

void Level::onPlayerHit(GameObject *player, int type)
{
    Player *p = (Player*) player;
    p->takeDamage(m_projectiles->prototype(type).damage());
}
