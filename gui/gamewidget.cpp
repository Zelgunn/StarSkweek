#include "gamewidget.h"

GameWidget::GameWidget(Game *game, QWidget *parent)
    : QWidget(parent), m_game(game)
{

}

void GameWidget::onRight()
{
    m_game->onRight();
}

void GameWidget::onUp()
{
    m_game->onUp();
}

void GameWidget::onLeft()
{
    m_game->onLeft();
}

void GameWidget::onDown()
{
    m_game->onDown();
}

void GameWidget::onEnter()
{
    m_game->onEnter();
}

void GameWidget::onBackpace()
{
    m_game->onBackpace();
}

void GameWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    paintGame(&painter);

    painter.end();
}

void GameWidget::paintGame(QPainter *painter)
{
    paintMap(painter);
    paintPlayer(painter);
    paintProjectiles(painter);
    paintHUD(painter);
}

void GameWidget::paintMap(QPainter *painter)
{
    const Tile *tile;
    const Level *level = m_game->level();
    const Grid *grid = level->grid();
    const Tile *tiles = level->tiles();
    QSize tileSize = level->tileSize();

    uint w = level->grid()->width();
    uint h = level->grid()->height();

    QSize mapSize;
    mapSize.setWidth(tileSize.width()*w);
    mapSize.setHeight(tileSize.height()*h);

    QPixmap map(mapSize);
    map.fill(QColor(0,0,0,0));
    QPainter mapPainter(&map);

    for(uint i=0; i<grid->width(); i++)
    {
        for(uint j=0; j<grid->height(); j++)
        {
            tile = &tiles[grid->tileAt(i,j)];
            if(tile->type() != Tile::Void)
                mapPainter.drawPixmap(i * tileSize.width(),
                                      j * tileSize.height(),
                                      tile->texture());
        }
    }

    const Player *player = level->player();

    painter->drawPixmap(toMap(player->position()), map);
}

void GameWidget::paintPlayer(QPainter *painter)
{
    QSize tileSize = m_game->level()->tileSize();

    QPixmap pImage;
    QPoint playerOnScreen(0.5 * width(), 0.5 * height());
    const Player *player;
    for(int i=0; i<m_game->players()->size(); i++)
    {
        player = m_game->players()->at(i);
        pImage = player->model()->scaled(tileSize.height(), tileSize.height());     // TMP
        playerOnScreen -= QPoint(pImage.width() / 2, pImage.height() /2);
        if(i==0)    // Cas du joueur sur cet ordinateur. (Ce trouve aussi être le cas du jeu solo).
        {
            painter->drawPixmap(playerOnScreen, pImage);
        }
        else
        {
            painter->drawPixmap(relativePosition(player->position(), pImage.size()), pImage);
        }
    }
}

void GameWidget::paintWaitingSign(QPainter *painter)
{
    static int dynamicWait = 0;
    const int dynamicSpeed = 30;

    QString message = "En attente de joueur";
    for(int i=0; i<dynamicWait/dynamicSpeed; i++) message = message.append('.');
    dynamicWait = ((dynamicWait + 1) % (dynamicSpeed * 4));

    QFont font("Times", 30, QFont::Bold);
    painter->setFont(font);

    int w = width() * 3/5;
    int h = height() / 10;
    QRect rect;
    rect.setX((width() - w)/2);
    rect.setY((height() - h)/2);
    rect.setWidth(w);
    rect.setHeight(h);

    painter->setBrush(QBrush(QColor(255,255,255,100)));

    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, message);
}

void GameWidget::paintProjectiles(QPainter *painter)
{
    const ProjectileList *projectiles = m_game->level()->projectiles();
    Projectile * projectile;

    QPixmap projectileModel;

    for(int i=0; i<projectiles->size(); i++)
    {
        projectile = projectiles->at(i);
        projectileModel = *projectile->model();
        painter->drawPixmap(relativePosition(projectile->position(), projectileModel.size()), projectileModel);
    }
}

void GameWidget::paintHUD(QPainter *painter)
{
    int w;
    const Player *player;
    for(int i=0; i<m_game->players()->size(); i++)
    {
        player = m_game->players()->at(i);
        QSize tileSize = m_game->level()->tileSize();

        Point position = player->position();
        position.y -= tileSize.height()/2 + 20;
        position.x -= tileSize.width()/2;

        w = tileSize.width();
        QRect lifeBarRect(relativePosition(position), QSize(w, 5));
        painter->setBrush(QColor(255,0,0));
        painter->drawRect(lifeBarRect);

        lifeBarRect.setWidth(w * player->lifeAnim()/player->maxLife());
        if(player->invulnerable())
            painter->setBrush(QColor(0,200,255));
        else
            painter->setBrush(QColor(200,255,0));
        painter->drawRect(lifeBarRect);

        if(player->life() <= player->lifeAnim())
        {
            lifeBarRect.setWidth(w * player->life()/player->maxLife());
            if(player->invulnerable())
                painter->setBrush(QColor(0,255,255));
            else
                painter->setBrush(QColor(0,255,0));
            painter->drawRect(lifeBarRect);
        }
    }
}


QPoint GameWidget::toMap(Point p)
{
    QPoint res;

    res.setX(width()/2 - p.x);
    res.setY(height()/2 - p.y);

    return res;
}

QPoint GameWidget::relativePosition(Point p, QSize size)
{
    Point playerPosition = m_game->level()->player()->position();
    QPoint playerOnScreen(0.5 * width(), 0.5 * height());

    QPoint deltaPos((playerPosition.x - p.x) + size.width()/2 + 1,
                    (playerPosition.y - p.y) + size.height()/2 + 1);

    return playerOnScreen - deltaPos;
}

void GameWidget::movePlayer(GameObject::Directions direction)
{
    m_game->movePlayer(direction);
}

