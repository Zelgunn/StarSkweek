#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "gameobject.h"

class Projectile : public GameObject
{
public:
    Projectile();
    Projectile(Directions direction, int ownerID);

    void move();

    const QImage *model() const;
    int ttl() const;

    int ownerID() const;
    void setOwnerID(int ownerID);

private:
    int m_ttl;
    int m_ownerID;
    QImage m_models[4];
};

#endif // PROJECTILE_H
