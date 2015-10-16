#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QDesktopWidget>
#include <QLinearGradient>
#include <QTime>

#include "windowappearance.h"
#include "game.h"
#include "tile.h"

#define PADDING 5

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
    void onGameReady();
    void onRight();
    void onUp();
    void onLeft();
    void onDown();
    void onEnter();

protected:
    void paintEvent(QPaintEvent *);
    void paintBackground(QPainter *painter);
    void paintBackgroundLaser(QPainter *painter);
    void paintGame(QPainter *painter);
    void paintMap(QPainter *painter);
    void paintPlayer(QPainter *painter);
    void paintWaitingSign(QPainter *painter);
    void paintProgressionBar(QPainter *painter);
    void paintProjectiles(QPainter *painter);
    void movePlayer(GameObject::Directions direction);

private:
    Game m_game;
    WindowAppearance m_appearance;
    QTimer *m_timer;
};

#endif // MAINWINDOW_H
