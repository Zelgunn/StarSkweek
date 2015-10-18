#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle("Super Skweek v0.1 (alpha)");
    setWindowState(Qt::WindowFullScreen);

    new QShortcut(tr("Right"), this, SLOT(onRight()));
    new QShortcut(tr("Left"), this, SLOT(onLeft()));
    new QShortcut(tr("Up"), this, SLOT(onUp()));
    new QShortcut(tr("Down"), this, SLOT(onDown()));
    new QShortcut(tr("Return"), this, SLOT(onEnter()));

    m_game.loadLevel(0);

    m_screenDim = QApplication::desktop()->screenGeometry().size();

    const Grid *grid = m_game.level()->grid();
    QSize tileSize = m_game.level()->tileSize();
    m_levelDim.setHeight(grid->height() * tileSize.height());
    m_levelDim.setWidth(grid->width() * tileSize.width());

    m_timer = new QTimer(this);
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    QObject::connect(&m_game, SIGNAL(gameReady()), this, SLOT(onGameReady()));
    m_timer->start(16);
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

//    QTime time;
//    static int count = 0; count ++;
//    static int sum = 0;
//    time = QTime::currentTime();

    paintBackground(painter);
    paintBackgroundLaser(painter);
    paintGame(painter);

    if(!m_game.isStarted())
        paintWaitingSign(painter);

//    sum += time.msecsTo(QTime::currentTime());
//    qDebug() << (double)sum / (double)count * 25 / 4;

    painter->end();
    delete painter;
}

void MainWindow::paintBackground(QPainter *painter)
{
    static QPixmap image(QApplication::applicationDirPath() + "/images/deathstar.png");
    static QList<QPoint*> stars;
    QPoint *star;
    static QList<int> starsSpeed;

    QRect rect = QApplication::desktop()->screenGeometry();
    int w = rect.width();
    int h = rect.height();

    painter->setBrush(QColor(0,0,0));
    painter->drawRect(rect);

    while(stars.size() < 1000)
    {
        stars.append(new QPoint(qrand()%w, qrand()%h));
        starsSpeed.append(qrand()%3 + 1);
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
            //starsSpeed.replace(i, qrand()%3 + 1);
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
    int tmp;

    QRect rect = QApplication::desktop()->screenGeometry();

    if(frame < cooldown)
    {
        p1 = ((double)(qrand()%100))/100.0;
        p2 = ((double)(qrand()%100))/100.0;
        frame = qrand()%5 + 5;
        htoh = qrand()%2;
        cooldown = - (qrand()%4)*60;
        tmp = qrand()%3;
        color = QColor((tmp==0)*255, (tmp==1)*255, (tmp==2)*255);
    }

    if(frame > 0)
    {

        QPen ppen = painter->pen();
        QPen pen(color);
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

void MainWindow::paintGame(QPainter *painter)
{
    paintMap(painter);
    paintPlayer(painter);
    paintProjectiles(painter);
}

void MainWindow::paintMap(QPainter *painter)
{
    const Tile *tile;
    const Grid *grid = m_game.level()->grid();
    const Tile *tiles = m_game.level()->tiles();
    QSize tileSize = m_game.level()->tileSize();

    QPixmap map(m_levelDim.width(), m_levelDim.height());
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
    QList<Projectile *> projectiles = m_game.level()->projectiles();
    Projectile * projectile;

    QPixmap projectileModel;

    for(int i=0; i<projectiles.size(); i++)
    {
        projectile = projectiles.at(i);
        projectileModel = *projectile->model();
        painter->drawPixmap(relativePosition(projectile->position(), projectileModel.size()), projectileModel);
    }
}

void MainWindow::onRight()
{
    movePlayer(GameObject::Right);
}

void MainWindow::onUp()
{
    movePlayer(GameObject::Up);
}

void MainWindow::onLeft()
{
    movePlayer(GameObject::Left);
}

void MainWindow::onDown()
{
    movePlayer(GameObject::Down);
}

void MainWindow::onEnter()
{
    if(!m_game.isStarted())
    {
        m_game.startGame();
        return;
    }
    m_game.playerFires(0);
}

void MainWindow::movePlayer(GameObject::Directions direction)
{
    m_game.movePlayer(direction);
}

QPoint MainWindow::toMap(Point p)
{
    QPoint res;

    res.setX(width()/2 - m_levelDim.width()*p.x);
    res.setY(height()/2 - m_levelDim.height()*p.y);

    return res;
}

QPoint MainWindow::relativePosition(Point p, QSize size)
{
    Point playerPosition = m_game.level()->player()->position();
    QPoint playerOnScreen(0.5 * m_screenDim.width(), 0.5 * m_screenDim.height());

    QPoint deltaPos((playerPosition.x - p.x)*m_levelDim.width() + size.width()/2,
                    (playerPosition.y - p.y)*m_levelDim.height() + size.height()/2);

    return playerOnScreen - deltaPos;
}
