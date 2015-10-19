#ifndef PROJECTILELIST_H
#define PROJECTILELIST_H

#include <QList>
#include "projectile.h"

class ProjectileList
{
public:
    ProjectileList();
    ProjectileList(QDomElement element);

    void append(int type, int faction, GameObject::Directions direction, Point position);
    void moveProjectiles(double ratio);

    void appendCollision(const GameObject* gameObject);
    Projectile *at(int i) const;
    int size() const;

private:
    QList<Projectile> m_prototypes; /** @brief Liste des prototypes de projectiles instanciables. */
    QList<Projectile *> m_projectiles; /** @brief Liste des projectiles instanciés et mutables. */
    QList<const GameObject *> m_gameObjects;
};

#endif // PROJECTILELIST_H
