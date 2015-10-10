#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QDesktopWidget>
#include <QLinearGradient>

#include "windowappearance.h"
#include "game.h"
#include "tile.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onRight();
    void onUp();
    void onLeft();
    void onDown();

protected:
    void paintEvent(QPaintEvent *);
    void paintMap(QPainter *painter);
    void paintPlayer(QPainter *painter);
    void movePlayer(GameObject::Directions direction);

private:
    Game m_game;
    WindowAppearance m_appearance;
    QList<Tile *> m_tiles;
    QTimer *m_timer;
};

#endif // MAINWINDOW_H
