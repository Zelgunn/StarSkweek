#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QPixmap>
#include <QApplication>
#include <QtCore/qmath.h>
#include <QTime>

#include "grid.h"

struct Point{
    int x,y;
};

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
    const QPixmap *model() const;
    void setModel(const QPixmap &model, Directions direction);
    Point position() const;
    void setPosition(const Point &position);
    void setPosition(int x, int y);
    void setSpeed(double speed);
    double speed() const;
    Directions direction() const;
    void setDirection(const Directions &direction);
    int faction() const;
    void setFaction(int faction);


    virtual bool isUnit() const;
    virtual bool isPlayer() const;
    virtual bool isMonster() const;
    virtual bool isProjectile() const;

    static Point displacement(Directions direction, double speed);
    static double euclidianDistance(const Point &p1, const Point &p2);


protected:
    Point m_position;
    Grid *m_grid;
    QPixmap m_models[4];
    QList<double> m_speed;
    Directions m_direction;
    int m_faction;
};

#endif // GAMEOBJECT_H
