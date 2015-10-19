#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "gameobject.h"

class Projectile : public GameObject
{
public:
    Projectile();
    Projectile(Directions direction, int faction);
    Projectile(const QDomElement &element);
    ~Projectile();

    void move(double ratio);

    const QPixmap *model() const;
    int ttl() const;

    int type() const;
    void setType(int type);

private:
    int m_ttl;
    int m_type;
};

#endif // PROJECTILE_H
