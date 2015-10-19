#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QPixmap>
#include <QApplication>
#include <QtCore/qmath.h>

#include "grid.h"

struct Point{
    double x,y;
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
    void setPosition(double x, double y);
    void setSpeed(double speed);
    double speed() const;
    Directions direction() const;
    void setDirection(const Directions &direction);
    int faction() const;
    void setFaction(int faction);

    static Point displacement(Directions direction, double speed, double ratio = 1.0);
    static double euclidianDistance(const Point &p1, const Point &p2);

protected:
    Point m_position;
    Grid *m_grid;
    QPixmap m_models[4];
    double m_speed;
    Directions m_direction;
    int m_faction;
};

#endif // GAMEOBJECT_H
