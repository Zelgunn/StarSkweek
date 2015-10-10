#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle("Super Skweek v0.0 (alpha)");
    setWindowState(Qt::WindowFullScreen);

    QString dir = QApplication::applicationDirPath();
    QString tmp = dir;
    tmp.append("/images/Tile_pink.png");
    m_tiles.append(new Tile(tmp));

    tmp = dir;
    tmp.append("/images/Tile_blue.png");
    m_tiles.append(new Tile(tmp));

    new QShortcut(tr("Right"), this, SLOT(onRight()));
    new QShortcut(tr("Left"), this, SLOT(onLeft()));
    new QShortcut(tr("Up"), this, SLOT(onUp()));
    new QShortcut(tr("Down"), this, SLOT(onDown()));

    const Grid *grid = m_game.level(0)->grid();
    QRect rect = QApplication::desktop()->screenGeometry();
    m_appearance.setAppearance(rect.width() - 10, rect.height(), grid->width(), grid->height());
}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter(this);

    paintMap(painter);
    paintPlayer(painter);

    painter->end();
}

void MainWindow::paintMap(QPainter *painter)
{
    Tile* tile;
    const Grid *grid = m_game.level(0)->grid();

    int theight = m_appearance.tileHeight();
    int twidth = m_appearance.tileWidth();
    int dy = m_appearance.dy()/2;

    for(uint i=0; i<grid->width(); i++)
    {
        for(uint j=0; j<grid->height(); j++)
        {
            tile = m_tiles.at(grid->tileAt(i,j));
            painter->drawImage(i * twidth + 5,
                              j * theight + dy,
                              tile->resizedTexture(twidth, theight));
        }
    }
}

void MainWindow::paintPlayer(QPainter *painter)
{
    int dy = m_appearance.dy()/2;
    int theight = m_appearance.tileHeight();

    const Player *player = m_game.level(0)->player();
    QImage pImage = player->appearance()->scaled(theight, theight);
    painter->drawImage(player->position().x * m_appearance.width() + 5 - pImage.width() / 2,
                      player->position().y * m_appearance.height()+ dy - pImage.height() / 2,
                      pImage);
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

void MainWindow::movePlayer(GameObject::Directions direction)
{
    m_game.movePlayer(direction);
    update();
}
