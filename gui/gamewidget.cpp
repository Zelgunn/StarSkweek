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

    updateBombs();
    paintGame(&painter);

    painter.end();
}

void GameWidget::paintGame(QPainter *painter)
{
    paintMap(painter);
    paintPlayer(painter);
    if(m_game->isPlayerDefeated() || m_game->isPlayerVictorious())
    {
        paintEndGamePanel(painter);
    }
    else
    {
        paintProjectiles(painter);
        paintAnimations(painter);
        paintBlackStarBeam(painter);
        paintHUD(painter);
        paintUI(painter);
    }
}

void GameWidget::paintMap(QPainter *painter)
{
    const Tile *tile;
    Level *level = m_game->level();
    Grid *grid = level->grid();
    Tile *tiles = level->tiles();
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
        pImage.save(QString::number(i) + ".png");
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
        else
        {
            QPixmap pixmap = animation->nextFrame();
            QPoint pos(animation->position());

            painter->drawPixmap(relativePosition(pos, pixmap.size(), false), pixmap);
        }
    }

    foreach(Animation *animation, animationsToDelete)
    {
        m_pendingAnimations.removeOne(animation);
        delete animation;
    }
}

void GameWidget::paintBlackStarBeam(QPainter *painter)
{
    DarthVader *darthVader = m_game->level()->darthVader();
    if(darthVader == Q_NULLPTR)
        return;

    if(!darthVader->blackStarActive())
        return;

    DeathStarBeam *deathStarBeam = darthVader->blackStarBeam();

    if(deathStarBeam->animationDone())
    {
        darthVader->deleteBlackStar();
        return;
    }

    QPixmap pixmap = deathStarBeam->nextFrame();
    QPoint pos(deathStarBeam->position());

    painter->drawPixmap(relativePosition(pos, pixmap.size(), false), pixmap);
}

void GameWidget::paintHUD(QPainter *painter)
{
    const Level *level = m_game->level();
    int w;
    Player *player;

    painter->setPen(QPen(QColor(0,0,0)));
    for(int i=0; i<level->players().size(); i++)
    {
        player = level->players().at(i);
        QSize tileSize = level->tileSize();

        QPoint position = player->position();
        if(player->isUpstairs()) position.ry() -= tileSize.height()*3/2;
        else position.ry() -= tileSize.height()*5/4;
        position.rx() -= tileSize.width()/2;

        w = tileSize.width();
        QRect lifeBarRect(relativePosition(position), QSize(w, 5));
        // Mode Fantôme
        if(player->isObiWan())
        {
            ObiWan *obiWan = (ObiWan*) player;
            QRect ghostFormBar(0,0, lifeBarRect.width() + 11, lifeBarRect.height() + 11);
            ghostFormBar.moveCenter(lifeBarRect.center());

            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(50,50,150,255 * qMin(obiWan->ghostFormTimeLeft(),1000)/1000));
            painter->drawRect(ghostFormBar);
            painter->setPen(QPen(QColor(0,0,0)));
        }

        // Vie (Totale)
        painter->setBrush(QColor(255,0,0));
        painter->drawRect(lifeBarRect);

        // Vie (Animée)
        lifeBarRect.setWidth(w * player->lifeAnim()/player->maxLife());
        if(player->invulnerable())
            painter->setBrush(QColor(0,200,255));
        else
            painter->setBrush(QColor(200,255,0));
        painter->drawRect(lifeBarRect);

        // Vie (Actuelle)
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
    paintScoreBar(painter);
    paintTimer(painter);
    paintPowerBar(painter);
}

void GameWidget::paintScoreBar(QPainter *painter)
{
    qreal playerRatio = m_game->level()->playerTileRatio(); // Ratio entre 0 et 1
    //Score rouge
    int redWidth = width()/2 * 100.0 * playerRatio / 100;
    int blueWidth = width()/2 - redWidth;
    QRect scoreRed(width()/4, 0, redWidth, height()/20);
    QLinearGradient redLightsaber(QPointF( 0, height()/40), QPointF( 0, 0));
    redLightsaber.setSpread(redLightsaber.ReflectSpread);
    redLightsaber.setColorAt(0, QColor(255,0,0,255));
    redLightsaber.setColorAt(1, QColor(255,0,0,20));
    QBrush red(QGradient &redLightsaber);
    painter->setPen(Qt::NoPen);
    painter->setBrush(redLightsaber);
    painter->drawRect(scoreRed);

    //Score bleu
    QRect scoreBlu(width()*3/4, 0, -blueWidth, height()/10/2);
    QLinearGradient bluLightsaber(QPointF( 0, height()/10/4), QPointF( 0, 0));
    bluLightsaber.setSpread(bluLightsaber.ReflectSpread);
    bluLightsaber.setColorAt(0, QColor(3,118,222,255));
    bluLightsaber.setColorAt(1, QColor(3,118,222,20));
    QBrush blu(QGradient &redbluPen);
    painter->setBrush(bluLightsaber);
    painter->drawRect(scoreBlu);

    //White Lightsaber effect ;)
    QRect scoreWhite(width()/4, 0, width()/2, height()/10/2);
    QLinearGradient whiteLightsaber(QPointF( 0, height()/10/4), QPointF( 0, height()/10/2));
    whiteLightsaber.setSpread(whiteLightsaber.ReflectSpread);
    whiteLightsaber.setColorAt(0, QColor(255,255,255,255));
    whiteLightsaber.setColorAt(0.4, QColor(255,255,255,0));
    whiteLightsaber.setColorAt(1, QColor(255,255,255,0));
    QBrush white(QGradient &redbluPen);
    painter->setBrush(whiteLightsaber);
    painter->drawRect(scoreWhite);

    //Sabres
    static QPixmap lightSaberPixmap(QApplication::applicationDirPath() + "/images/Anakin Saber-01.png");
    QRect redSaber(width()/8,0,width()/7,height()/10/2);
    painter->drawPixmap(redSaber,lightSaberPixmap);

    static QPixmap lightSaber2Pixmap(QApplication::applicationDirPath() + "/images/Obi wan Lightsaber.png");
    QRect bluSaber(width()*3/4,0,width()/7,height()/10/2);
    painter->drawPixmap(bluSaber,lightSaber2Pixmap);
}

void GameWidget::paintTimer(QPainter *painter)
{
    const int fontSize = 24;
    QTime now = QTime::currentTime();
    int secs = GAME_DURATION - (m_game->startTime().msecsTo(now)/1000);
    if(secs < 0) secs = 0;
    int mins = secs/60;
    secs %= 60;

    painter->setFont(QFont("Times", fontSize));
    painter->setPen(QColor(255,255,255,255));

    static int blink = 0;
    if(mins == 0)
    {
        blink = (blink + 1)%60;
        painter->setPen(QColor(255,0,0,255));
    }
    else
        blink = 0;

    QString secsString = QString::number(secs);
    if(secs < 10) secsString.insert(-1, '0');

    painter->drawText(QRect(0, height()/20, width()/2 - fontSize/4, height()/20), Qt::AlignRight, QString::number(mins));
    if(blink <= 30)
        painter->drawText(QRect(0, height()/20, width(), height()/20), Qt::AlignHCenter, ":");
    painter->drawText(QRect(width()/2 + fontSize/4, height()/20, width()/2 - fontSize/4, height()/20), Qt::AlignLeft, secsString);
}

void GameWidget::paintPowerBar(QPainter *painter)
{
    QRectF rectanglePower(-width()*6.25/100, height()/9*8, width()*12.5/100, height()*22.22/100);
    int startAngle = 0 * 16;
    int spanAngle = 90 * 16;
    QPen pen2(QColor(33,99,131));
    pen2.setWidth(2);
    painter->setPen(pen2);
    painter->setBrush(Qt::NoBrush);
    painter->drawPie(rectanglePower, startAngle, spanAngle);

    //Affichage des 5 étapes de PowerUp
    qreal powerRatio = m_game->level()->player()->powerRessourceRatio() * 5;
    for(int i=0; i<5 ; i++)
    {
        startAngle = i*18 * 16;
        spanAngle = (i+1)*18 * 16;
        QPen pen3(QColor(73,212,253));
        pen3.setWidth(3);
        painter->setPen(pen3);

        // Chargement de la barre en fonction du Power
        if((i+1) < powerRatio)
        {
            painter->setBrush(QColor(51,148,193));
        }
        else if(i  < powerRatio)
        {
            painter->setBrush(QColor(73 - 22*((qreal)powerRatio - i),
                                     212 - 64*((qreal)powerRatio - i),
                                     253 - 60*((qreal)powerRatio - i)));
        }
        else
        {
            painter->setBrush(QColor(73,212,253));
        }

        painter->drawPie(rectanglePower,startAngle,spanAngle);
    }

    QRectF rectangleBlack(-width()*2.6/100, height()/9*8.5, width()*5.2/100, height()*9.26/100);
    painter->setBrush(QColor(73,212,253,255));
    painter->drawEllipse(rectangleBlack);

    //Midichloriens
    static QPixmap midCh(QApplication::applicationDirPath() + "/images/power.png");
    QRect power(-5, height()/9*8.60, width()*12.5/100/4, height()*22.22/100/4);
    painter->drawPixmap(power,midCh);
}

void GameWidget::paintEndGamePanel(QPainter *painter)
{
    QString message;
    if(m_game->isPlayerDefeated())
        message = "Défaite";
    else
        message = "Victoire !";


    int w = width() * 3/5;
    int h = height() / 10;
    QRect rect;
    rect.setX((width() - w)/2);
    rect.setY((height() - h)/2);
    rect.setWidth(w);
    rect.setHeight(h);

    painter->setFont(QFont("Times", 30, QFont::Bold));
    painter->setBrush(QBrush(QColor(0,0,0,200)));
    painter->setPen(QPen(QColor(255,255,255)));

    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, message);
}

QPoint GameWidget::toMap(QPoint p)
{
    QPoint res;

    res.setX(width()/2 - p.x());
    res.setY(height()/2 - p.y());

    return res;
}

QPoint GameWidget::relativePosition(QPoint p, QSize size, bool usePlayerSize)
{
    QPoint playerPosition = m_game->level()->player()->position();
    QPoint playerOnScreen(0.5 * width(), 0.5 * height());
    QSize tileSize = m_game->level()->tileSize();

    if(usePlayerSize)
    {
        if(m_game->level()->player()->isUpstairs())
        {
            playerOnScreen -= QPoint(0, tileSize.height() * 7/8 - 5);
        }

        else
        {
            playerOnScreen -= QPoint(0, tileSize.height() * 3/4 - 5);
        }
    }

    QPoint deltaPos((playerPosition.x() - p.x()) + size.width()/2 + 1,
                    (playerPosition.y() - p.y()) + size.height()/2 + 1);

    return playerOnScreen - deltaPos;
}

void GameWidget::movePlayer(GameObject::Directions direction)
{
    m_game->movePlayer(direction);
}

void GameWidget::updateBombs()
{
    Level *level = m_game->level();
    Player *player;
    Grid *grid = level->grid();
    QSize tileSize = level->tileSize();
    int w = tileSize.width(), h = tileSize.height();
    Tile::TileType tileType;
    int x,y;
    QPoint position;

    for(int i=0; i<level->players().size(); i++)
    {
        player = level->player(i);
        x = player->position().x()/w;
        y = player->position().y()/h;
        tileType = grid->tileAt(x, y);
        if(tileType == Tile::BombTile)
        {
            position = QPoint(x*w + w/2, y*h + h/2);
            grid->setTileAt(x, y, Tile::ExplodingTile);
            m_pendingAnimations.append(new Bombtile(grid, position, tileSize));
        }
    }
}
