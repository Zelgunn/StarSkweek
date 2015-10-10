#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QImage>
#include <QApplication>

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
        Down
    };

    GameObject();
    const QImage *appearance() const;
    Point position() const;
    void setPosition(const Point &position);
    void setPosition(double x, double y);
    double speed() const;

    static Point displacement(Directions direction, double speed);

protected:
    Point m_position;
    Grid *m_grid;
    QImage m_appearance;
    double m_speed;
};

#endif // GAMEOBJECT_H
