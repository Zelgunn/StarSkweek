#ifndef PROJECTILELIST_H
#define PROJECTILELIST_H

#include <QList>
#include "projectile.h"

class ProjectileList : public QObject
{
    Q_OBJECT
public:
    ProjectileList(QObject *parent = Q_NULLPTR);
    ProjectileList(QDomElement element, QObject *parent = Q_NULLPTR);

    void append(int type, int faction, GameObject::Directions direction, QPoint position);
    void moveProjectiles();

    void appendCollision(GameObject* gameObject);
    Projectile *at(int i) const;
    int size() const;
    Projectile prototype(int i) const;

signals:
    void hitPlayer(GameObject* player, int type);

private:
    QList<Projectile> m_prototypes; /** @brief Liste des prototypes de projectiles instanciables. */
    QList<Projectile *> m_projectiles; /** @brief Liste des projectiles instanciés et mutables. */
    QList<GameObject *> m_gameObjects;
};

#endif // PROJECTILELIST_H
