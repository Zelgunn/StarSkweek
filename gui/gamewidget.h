#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>

#include "game.h"
#include "animations/animation.h"
#include "animations/deathstarbeam.h"
#include "animations/bombtile.h"

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
    // Dessin
    void paintEvent(QPaintEvent *);
    void paintGame(QPainter *painter);
    void paintMap(QPainter *painter);
    void paintPlayer(QPainter *painter);
    void paintWaitingSign(QPainter *painter);
    void paintProjectiles(QPainter *painter);
    void paintAnimations(QPainter *painter);
    void paintBlackStarBeam(QPainter *painter);
    void paintHUD(QPainter *painter);
    void paintUI(QPainter *painter);
    void paintScoreBar(QPainter *painter);
    void paintTimer(QPainter *painter);
    void paintPowerBar(QPainter *painter);
    void paintEndGamePanel(QPainter *painter);

    // Fonctions de position
    QPoint toMap(QPoint p);
    QPoint relativePosition(QPoint p, QSize size = QSize(0,0), bool usePlayerSize = true);
    void movePlayer(GameObject::Directions direction);

    // Mises à jours
    void updateBombs();

private:
    Game *m_game;
    QList <Animation *> m_pendingAnimations;
};

#endif // GAMEWIDGET_H
