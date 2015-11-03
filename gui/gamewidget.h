#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>

#include "game.h"

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(Game* game, QWidget *parent = 0);

    void onRight();
    void onUp();
    void onLeft();
    void onDown();
    void onEnter();
    void onBackpace();

protected:
    void paintEvent(QPaintEvent *);
    void paintGame(QPainter *painter);
    void paintMap(QPainter *painter);
    void paintPlayer(QPainter *painter);
    void paintWaitingSign(QPainter *painter);
    void paintProjectiles(QPainter *painter);
    void paintHUD(QPainter *painter);

    QPoint toMap(Point p);
    QPoint relativePosition(Point p, QSize size = QSize(0,0));
    void movePlayer(GameObject::Directions direction);

private:
    Game *m_game;
};

#endif // GAMEWIDGET_H
