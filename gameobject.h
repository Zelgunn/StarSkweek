#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QPixmap>
#include <QApplication>
#include <QtCore/qmath.h>
#include <QTime>

#include "grid.h"

class GameObject
{
public:
    enum Directions{
        Right,
        Up,
        Left,
        Down,
        InvalidDirection = -1
    };

    GameObject();

    // Accesseurs et mutateurs
    const QPixmap *model() const;
    void setModel(const QPixmap &model, Directions direction);
    QPoint position() const;
    void setPosition(const QPoint &position);
    void setPosition(int x, int y);
    void setSpeed(qreal speed);
    void setSpeed(const QList<qreal> &speed);
    qreal speed() const;
    Directions direction() const;
    void setDirection(const Directions &direction);
    int faction() const;
    void setFaction(int faction);
    bool isUpstairs() const;
    void setUpstairs(bool upstairs);

    // Fonction virtuelles de typage
    virtual bool isUnit() const;
    virtual bool isPlayer() const;
    virtual bool isMonster() const;
    virtual bool isProjectile() const;

    // Fonctions statiques
    static QPoint displacement(Directions direction, qreal speed);
    static double euclidianDistance(const QPoint &p1, const QPoint &p2);

    Grid *grid() const;
    void setGrid(Grid *grid);

protected:
    QPoint m_position;
    Grid *m_grid;
    QPixmap m_models[4];
    QList<qreal> m_speed;
    Directions m_direction;
    int m_faction;
    bool m_upstairs;
};

#endif // GAMEOBJECT_H
