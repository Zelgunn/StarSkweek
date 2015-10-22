#ifndef UNIT_H
#define UNIT_H

#include <QTimer>
#include "gameobject.h"
#include "weapon.h"

class Unit : public QObject, public GameObject
{
    Q_OBJECT
public:
    Unit();

    int life() const;
    int maxLife() const;
    int lifeAnim() const;
    void updateLifeAnim();
    bool dead() const;
    void setDead(bool dead);

    void takeDamage(int damage);

    bool isUnit() const;

    bool invulnerable() const;
    void setInvulnerable(bool invulnerable);

public slots:
    void resurrect();
    void removeInvulnerability();

protected:
    int m_life;
    int m_maxLife;
    int m_lifeAnim;
    bool m_dead;
    int m_deathDuration;
    bool m_isResurecting;

    bool m_invulnerable;
    int m_invulnerabilityDuration;
};

#endif // UNIT_H
