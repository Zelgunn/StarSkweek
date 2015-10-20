#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "gameobject.h"

class Projectile : public GameObject
{
public:
    Projectile();
    Projectile(const QDomElement &element);
    virtual ~Projectile();

    void move();

    const QPixmap *model() const;
    int ttl() const;

    int type() const;
    void setType(int type);

    bool isProjectile() const;

    int damage() const;
    void setDamage(int damage);

private:
    int m_ttl;
    int m_type;
    int m_damage;
};

#endif // PROJECTILE_H
