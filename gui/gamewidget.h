#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>

#include "game.h"
#include "animation.h"
#include "deathstarbeam.h"

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
    void paintAnimations(QPainter *painter);
    void paintHUD(QPainter *painter);
    void paintUI(QPainter *painter);
    void paintEndGamePanel(QPainter *painter);

    QPoint toMap(QPoint p);
    QPoint relativePosition(QPoint p, QSize size = QSize(0,0));
    void movePlayer(GameObject::Directions direction);

private:
    Game *m_game;
    QList <Animation *> m_pendingAnimations;
};

#endif // GAMEWIDGET_H
