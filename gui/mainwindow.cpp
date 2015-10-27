#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QStackedWidget(parent)
{
    setWindowTitle("Super Skweek v0.1 (alpha)");
    setWindowState(Qt::WindowFullScreen);

    new QShortcut(tr("Right"), this, SLOT(onRight()));
    new QShortcut(tr("Left"), this, SLOT(onLeft()));
    new QShortcut(tr("Up"), this, SLOT(onUp()));
    new QShortcut(tr("Down"), this, SLOT(onDown()));
    new QShortcut(tr("Return"), this, SLOT(onEnter()));
    new QShortcut(tr("Backspace"), this, SLOT(onBackpace()));

    m_game.loadLevel(0);

    m_screenDim = QApplication::desktop()->screenGeometry().size();

    m_timer = new QTimer(this);
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    QObject::connect(&m_game, SIGNAL(gameReady()), this, SLOT(onGameReady()));
    m_timer->start(16);

    // Widgets
    m_menuWidget = new MainMenuWidget(this);
    addWidget(m_menuWidget);
    QObject::connect(m_menuWidget, SIGNAL(onExit()), this, SLOT(close()));

    setCurrentIndex(0);

    // Musique
    m_musicPlayer = new QMediaPlayer;

    m_playlist = new QMediaPlaylist;
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    m_playlist->addMedia(QUrl("qrc:/musics/musics/Main.mp3"));
    m_musicPlayer->setVolume(50);

    m_musicPlayer->setPlaylist(m_playlist);
    m_musicPlayer->play();
}

MainWindow::~MainWindow()
{

}

void MainWindow::onGameReady()
{
    m_game.startGame();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter(this);
    paintBackground(painter);

    switch(m_game.state())
    {
    case Game::MenuState:
        break;
    case Game::LobbyState:
        paintLobby(painter);
        break;
    case Game::PlayingState:
        paintGame(painter);
        break;
    }

    painter->end();
    delete painter;
}

void MainWindow::paintLobby(QPainter *painter)
{
    paintBackground(painter);
}

void MainWindow::paintGame(QPainter *painter)
{
    static QTime time = QTime::currentTime();;
    static int count = 0; count ++;
    static int sum = 0;

    paintBackground(painter);
    paintBackgroundLaser(painter);
    paintMap(painter);
    paintPlayer(painter);
    paintProjectiles(painter);
    paintHUD(painter);

    sum += time.msecsTo(QTime::currentTime());
    time = QTime::currentTime();

    painter->setFont(QFont("Times", 16, QFont::Bold));
    painter->drawText(width() - 150, 50, "FPS : " + QString::number(1000*count/sum));
}

void MainWindow::paintBackground(QPainter *painter)
{
    static QPixmap image(QApplication::applicationDirPath() + "/images/deathstar.png");
    static QList<QPoint*> stars;
    QPoint *star;
    static QList<int> starsSpeed;
    static int deathstarMove = 0;
    deathstarMove ++;

    QRect rect = QApplication::desktop()->screenGeometry();
    int w = rect.width();
    int h = rect.height();

    painter->setBrush(QColor(0,0,0));
    painter->drawRect(rect);

    while(stars.size() < 1000)
    {
        stars.append(new QPoint(qrand()%w, qrand()%h));
        if(stars.size() == 1)
            starsSpeed.append(0);
        else
            starsSpeed.append((qrand()%12 + 1));
    }

    QPoint *tmp;
    painter->setBrush(QColor(255,255,255));
    painter->setPen(QColor(255,255,255));

    for(int i = 0; i < stars.size(); i++)
    {
        tmp = stars.at(i);
        painter->drawPoint(*tmp);
        tmp->setX(tmp->x() - starsSpeed.at(i));
        if(i==0)
        {
            if(deathstarMove == 5)
            {
                deathstarMove = 0;
                tmp->setX(tmp->x() - 1);
            }
            painter->drawPixmap(*tmp, image);
            if(tmp->x() < - image.width())
            {
                tmp->setY(qrand()%h - image.height()/2);
                tmp->setX(w);
            }
        }
        else if(tmp->x() < 0)
        {
            star = stars.at(i);
            star->setX(w);
            star->setY(qrand()%h);
        }
    }
}

void MainWindow::paintBackgroundLaser(QPainter *painter)
{
    static int frame = -1;
    static double p1, p2;
    static int htoh;
    static int cooldown = -60;
    static QColor color;
    static int size;
    int tmp;

    QRect rect = QApplication::desktop()->screenGeometry();

    if((frame < cooldown) && frame < 0)
    {
        p1 = ((double)(qrand()%100))/100.0;
        p2 = ((double)(qrand()%100))/100.0;
        frame = qrand()%12 + 5;
        size = frame;
        htoh = qrand()%2;
        cooldown = - (qrand()%16)*60 + 320;
        tmp = qrand()%3;
        color = QColor((tmp==0)*255, (tmp==1)*255, (tmp==2)*255);
    }

    if(frame > 0)
    {
        QPen ppen = painter->pen();
        QPen pen(color);
        if(frame > size/2)
            pen.setWidth(size - frame);
        else
            pen.setWidth(frame);
        painter->setPen(pen);

        if(htoh == 1)
        {
            painter->drawLine(p1 * rect.width(),
                              0,
                              p2 * rect.width(),
                              rect.height());

        }
        else
        {
            painter->drawLine(0,
                              p1 * rect.width(),
                              rect.width(),
                              p2 *rect.height());
        }

        painter->setPen(ppen);
    }
    frame --;
}

void MainWindow::paintMap(QPainter *painter)
{
    const Tile *tile;
    const Grid *grid = m_game.level()->grid();
    const Tile *tiles = m_game.level()->tiles();
    QSize tileSize = m_game.level()->tileSize();

    uint w = m_game.level()->grid()->width();
    uint h = m_game.level()->grid()->height();

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

    const Player *player = m_game.level()->player();

    painter->drawPixmap(toMap(player->position()), map);
}

void MainWindow::paintPlayer(QPainter *painter)
{
    QSize tileSize = m_game.level()->tileSize();

    const Player *player = m_game.level()->player();
    const Player *player2 = m_game.level()->player2();

    QPixmap pImage = player->model()->scaled(tileSize.height(), tileSize.height());
    QPoint playerOnScreen(0.5 * m_screenDim.width() - pImage.width()/2,
                          0.5 * m_screenDim.height() - pImage.height()/2);
    painter->drawPixmap(playerOnScreen, pImage);

    pImage = player2->model()->scaled(tileSize.height(), tileSize.height());
    painter->drawPixmap(relativePosition(player2->position(), pImage.size()), pImage);
}

void MainWindow::paintWaitingSign(QPainter *painter)
{
    static int dynamicWait = 0;
    const int dynamicSpeed = 30;

    QString message = "En attente de joueur";
    for(int i=0; i<dynamicWait/dynamicSpeed; i++) message = message.append('.');
    dynamicWait = ((dynamicWait + 1) % (dynamicSpeed * 4));

    QFont font("Times", 30, QFont::Bold);
    painter->setFont(font);

    int w = m_screenDim.width() * 3/5;
    int h = m_screenDim.height() / 10;
    QRect rect;
    rect.setX((m_screenDim.width() - w)/2);
    rect.setY((m_screenDim.height() - h)/2);
    rect.setWidth(w);
    rect.setHeight(h);

    painter->setBrush(QBrush(QColor(255,255,255,100)));

    painter->drawRect(rect);
    painter->drawText(rect, Qt::AlignCenter, message);
}

void MainWindow::paintProjectiles(QPainter *painter)
{
    const ProjectileList *projectiles = m_game.level()->projectiles();
    Projectile * projectile;

    QPixmap projectileModel;

    for(int i=0; i<projectiles->size(); i++)
    {
        projectile = projectiles->at(i);
        projectileModel = *projectile->model();
        painter->drawPixmap(relativePosition(projectile->position(), projectileModel.size()), projectileModel);
    }
}

void MainWindow::paintHUD(QPainter *painter)
{
    int w;
    for(int i=0; i<2; i++)
    {
        const Player *player = m_game.level()->player(i);

        QSize tileSize = m_game.level()->tileSize();

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

void MainWindow::onRight()
{
    movePlayer(GameObject::Right);
}

void MainWindow::onUp()
{
    switch(m_game.state())
    {
    case Game::MenuState:
        m_menuWidget->aboveMenu();
        break;
    case Game::LobbyState:
        break;
    case Game::PlayingState:
        movePlayer(GameObject::Up);
        break;
    }
}

void MainWindow::onLeft()
{
    movePlayer(GameObject::Left);
}

void MainWindow::onDown()
{
    switch(m_game.state())
    {
    case Game::MenuState:
        m_menuWidget->belowMenu();
        break;
    case Game::LobbyState:
        break;
    case Game::PlayingState:
        movePlayer(GameObject::Down);
        break;
    }
}

void MainWindow::onEnter()
{
    switch(m_game.state())
    {
    case Game::MenuState:
        m_menuWidget->selectMenu();
        break;
    case Game::LobbyState:
        break;
    case Game::PlayingState:
        m_game.playerFires(0);
        break;
    }
}

void MainWindow::onBackpace()
{
    switch(m_game.state())
    {
    case Game::MenuState:
        m_menuWidget->previousMenu();
        break;
    case Game::LobbyState:
        break;
    case Game::PlayingState:
        break;
    }
}

void MainWindow::movePlayer(GameObject::Directions direction)
{
    m_game.movePlayer(direction);
}

QPoint MainWindow::toMap(Point p)
{
    QPoint res;

    res.setX(width()/2 - p.x);
    res.setY(height()/2 - p.y);

    return res;
}

QPoint MainWindow::relativePosition(Point p, QSize size)
{
    Point playerPosition = m_game.level()->player()->position();
    QPoint playerOnScreen(0.5 * m_screenDim.width(), 0.5 * m_screenDim.height());

    QPoint deltaPos((playerPosition.x - p.x) + size.width()/2 + 1,
                    (playerPosition.y - p.y) + size.height()/2 + 1);

    return playerOnScreen - deltaPos;
}
