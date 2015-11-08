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

    // Fonctions statiques
    static QPoint displacement(Directions direction, qreal speed, qreal angle);

private:
    int m_ttl;
    int m_type;
    int m_damage;
    QList<qreal> m_angle;
};

#endif // PROJECTILE_H
