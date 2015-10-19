#include "projectilelist.h"

ProjectileList::ProjectileList()
{

}

ProjectileList::ProjectileList(QDomElement element)
{
    QDomNode node = element.firstChild();

    while(!node.isNull())
    {
        m_prototypes.append(Projectile(node.toElement()));

        node = node.nextSibling();
    }
}

void ProjectileList::append(int type, int faction, GameObject::Directions direction, Point position)
{
   if((type < 0)||(type >= m_prototypes.size()))
       return;

   Projectile *copy = new Projectile(m_prototypes.at(type));
   copy->setDirection(direction);
   copy->setFaction(faction);
   copy->setPosition(position);

   m_projectiles.append(copy);
}

void ProjectileList::moveProjectiles(double ratio)
{
    Projectile *projectile, *projectile2;
    const GameObject *gameObject;

    for(int i=0; i<m_projectiles.size(); i++)
    {
        projectile = m_projectiles.at(i);
        projectile->move(ratio);
        if(projectile->ttl() < 0)
        {
            m_projectiles.removeAt(i);
            delete projectile;
            i--;
        }
        else
        {
            for(int j=0; j<m_gameObjects.size(); j++)
            {
                gameObject = m_gameObjects.at(j);
                if(gameObject->faction() != projectile->faction())
                {
                    if(GameObject::euclidianDistance(gameObject->position(), projectile->position()) < 0.02)
                    {
                        m_projectiles.removeAt(i);
                        delete projectile;
                        i--;
                    }
                }
            }

            for(int j=0; j<m_projectiles.size(); j++)
            {
                projectile2 = m_projectiles.at(j);
                if(projectile2->faction() != projectile->faction())
                {
                    m_projectiles.removeOne(projectile);
                    m_projectiles.removeOne(projectile2);
                    delete projectile;
                    delete projectile2;
                    i--;
                    j--;
                }
            }
        }
    }
}

void ProjectileList::appendCollision(const GameObject *gameObject)
{
    m_gameObjects.append(gameObject);
}

Projectile *ProjectileList::at(int i) const
{
    return m_projectiles.at(i);
}

int ProjectileList::size() const
{
    return m_projectiles.size();
}

