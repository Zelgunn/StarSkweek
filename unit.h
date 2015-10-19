#ifndef UNIT_H
#define UNIT_H

#include "gameobject.h"
#include "weapon.h"

class Unit : public GameObject
{
public:
    Unit();

    int life() const;
    int maxLife() const;
    int lifeAnim() const;
    void updateLifeAnim();

    void takeDamage(int damage);

    bool isUnit() const;

protected:
    int m_life;
    int m_maxLife;
    int m_lifeAnim;
};

#endif // UNIT_H
