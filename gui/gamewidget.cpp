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
    if(m_game->state() == Game::PlayingState)
    {
        m_pendingAnimations.append(new DeathStarBeam(m_game->level()->grid(), m_game->level()->tileSize(), 0));
    }
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
    paintAnimations(painter);
    paintHUD(painter);
    paintUI(painter);
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
    const Level *level = m_game->level();
    QSize tileSize = m_game->level()->tileSize();

    QPixmap pImage;
    QPoint playerOnScreen(0.5 * width(), 0.5 * height()), position;
    const Player *player;
    for(int i=0; i<level->players().size(); i++)
    {
        player = level->players().at(i);
        if(player->isUpstairs())
        {
            pImage = player->model()->scaledToHeight(tileSize.height() * 7/4, Qt::SmoothTransformation);     // TMP
        }

        else
        {
            pImage = player->model()->scaledToHeight(tileSize.height() * 3/2, Qt::SmoothTransformation);     // TMP
        }
        playerOnScreen -= QPoint(pImage.width() / 2, pImage.height() - 10);
        if(i==0)    // Cas du joueur sur cet ordinateur. (Ce trouve aussi être le cas du jeu solo).
        {
            position = playerOnScreen;
        }
        else
        {
            position = relativePosition(player->position(), pImage.size());
        }
        if(player->isUpstairs())
            position -= QPoint(0, tileSize.height()/4);
        painter->drawPixmap(position, pImage);
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

void GameWidget::paintAnimations(QPainter *painter)
{
    QList <Animation *> animationsToDelete;

    foreach(Animation *animation, m_pendingAnimations)
    {
        if(animation->animationDone())
        {
            animationsToDelete.append(animation);
        }
        QPixmap pixmap = animation->nextFrame();
        Point pos;
        pos.x = animation->position().x();
        pos.y = animation->position().y();

        painter->drawPixmap(relativePosition(pos, pixmap.size()), pixmap);
    }

    foreach(Animation *animation, animationsToDelete)
    {
        m_pendingAnimations.removeOne(animation);
        delete animation;
    }
}

void GameWidget::paintHUD(QPainter *painter)
{
    const Level *level = m_game->level();
    int w;
    const Player *player;

    painter->setPen(QPen(QColor(0,0,0)));
    for(int i=0; i<level->players().size(); i++)
    {
        player = level->players().at(i);
        QSize tileSize = level->tileSize();

        Point position = player->position();
        if(player->isUpstairs()) position.y -= tileSize.height()*3/2;
        else position.y -= tileSize.height()*5/4;
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

void GameWidget::paintUI(QPainter *painter)
{
    QRect scoreRect(width()/4, 0, width()/2, height()/10/2);
    QPen pen(QColor(50,50,50));
    pen.setWidth(3);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(scoreRect);

    //QRect scoreRed(width()/4, 0, width()/4, height()/10/2);


    QRectF rectanglePower(-120.0, height()/9*8, 240.0, 240.0);
    int startAngle = 0 * 16;
    int spanAngle = 90 * 16;
    QPen pen2(QColor(33,99,131));
    pen2.setWidth(2);
    painter->setPen(pen2);
    painter->setBrush(Qt::NoBrush);
    painter->drawPie(rectanglePower, startAngle, spanAngle);

    //Affichage des 5 étapes de PowerUp
    for(int i=0; i<5 ; i++)
    {
        startAngle = i*18 * 16;
        spanAngle = (i+1)*18 * 16;
        QPen pen3(QColor(73,212,253));
        pen3.setWidth(3);
        painter->setPen(pen3);
        painter->setBrush(QColor(51,148,193));
        painter->drawPie(rectanglePower,startAngle,spanAngle);
    }

    QRectF rectangleBlack(-50, height()/9*8.5, 100, 100);
    painter->setBrush(QColor(73,212,253,255));
    painter->drawEllipse(rectangleBlack);
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
    QSize tileSize = m_game->level()->tileSize();

    if(m_game->level()->player()->isUpstairs())
    {
        playerOnScreen -= QPoint(0, tileSize.height() * 7/8 - 5);
    }

    else
    {
        playerOnScreen -= QPoint(0, tileSize.height() * 3/4 - 5);
    }

    QPoint deltaPos((playerPosition.x - p.x) + size.width()/2 + 1,
                    (playerPosition.y - p.y) + size.height()/2 + 1);

    return playerOnScreen - deltaPos;
}

void GameWidget::movePlayer(GameObject::Directions direction)
{
    m_game->movePlayer(direction);
}

