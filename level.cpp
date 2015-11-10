#include "level.h"

Level::Level(const QDomElement &element, QList<Player *> *prototypes, const QList<PlayerInfo *> &playersInfos) :
    m_myPlayer(0)
{
    PlayerInfo *playerInfo;
    for(int i=0; i<playersInfos.size(); i++)
    {
        playerInfo = playersInfos.at(i);
        m_players.append(prototypes->at(playerInfo->characterSelected()));
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
    for(int i=0; i<playersInfos.size(); i++)
    {
        player = m_players.at(i);

        // Réglage de la vitesse
        player->setSpeed(qPow(m_tileSize.width() * m_tileSize.height(), 0.25));

        // Réglage de la faction
        player->setFaction(i);

        // Réglage de l'arme
        player->setWeapon(m_weapons.at(0));

        // Ajout du joueur dans la liste des objets à collision
        m_projectiles->appendCollision(player);

        // Si Dark Vador, renseigner la taille d'une case pour le rayon de l'étoile noire.
        if(player->isDarthVader())
        {
            DarthVader *darthVader = (DarthVader*) player;
            darthVader->setTileSize(m_tileSize);
        }

        // Renseignement de la grille
        player->setGrid(m_grid);
    }

    //if(playersInfos.size() == 1) initGridPlayerTiles();
    QObject::connect(m_projectiles, SIGNAL(hitPlayer(GameObject*,int)), this, SLOT(onPlayerHit(GameObject*,int)));
}

void Level::setMyPlayer(int playerNumber)
{
    m_myPlayer = playerNumber;
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
    initGridPlayerTiles();
}

void Level::initGridPlayerTiles()
{
    // On récupère le personnage du joueur hôte.
    Player *player = m_players.at(m_myPlayer);
    // Si le joueur ne joue pas Obiwan, on change
    if(player->isDarthVader())
    {
        m_grid->swapTiles(Tile::Player1Tile, Tile::TypeCount);
        m_grid->swapTiles(Tile::Player2Tile, Tile::Player1Tile);
        m_grid->swapTiles(Tile::TypeCount, Tile::Player2Tile);
    }
}

Grid *Level::grid()
{
    return m_grid;
}

Player *Level::player(int index) const
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
    if(ObiWan::isObiWanAndGhost(player)) return;

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
        if((nextTileType == Tile::Void) && (!ObiWan::isObiWanAndGhost(player)))
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
        Tile::TileType otherType = Tile::Player1Tile;
        if(player->tileType() == Tile::Player1Tile)
            otherType = Tile::Player2Tile;

        if(m_grid->tileAt(tx, ty) == otherType)
        {
            m_grid->setTileAt(tx, ty, player->tileType());
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

ObiWan *Level::obiWan() const
{
    foreach (Player *player, m_players) {
        if(player->isObiWan())
            return (ObiWan*) player;
    }

    return Q_NULLPTR;
}

DarthVader *Level::darthVader() const
{
    foreach (Player *player, m_players) {
        if(player->isDarthVader())
            return (DarthVader*) player;
    }

    return Q_NULLPTR;
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
        player->regeneratePower();
    }
}

void Level::onPlayerHit(GameObject *player, int type)
{
    Player *p = (Player*) player;
    p->takeDamage(m_projectiles->prototype(type).damage());
}
