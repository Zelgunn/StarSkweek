#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QDesktopWidget>
#include <QLinearGradient>
#include <QTime>

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
    void paintProjectiles(QPainter *painter);
    void movePlayer(GameObject::Directions direction);

    QPoint toMap(Point p);
    QPoint relativePosition(Point p, QSize size = QSize(0,0));

private:
    Game m_game;
    QTimer *m_timer;
    QSize m_screenDim;
    QSize m_levelDim;
};

#endif // MAINWINDOW_H
