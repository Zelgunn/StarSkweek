#include "unit.h"

Unit::Unit()
{
    m_position.x = 0;
    m_position.y = 0;

    m_maxLife = 100;
    m_life = m_maxLife;
    m_lifeAnim = m_maxLife;
    m_dead = false;
    m_deathDuration = 1000;
    m_isResurecting = false;

    m_invulnerable = false;
    m_invulnerabilityDuration = 3000;
}
int Unit::life() const
{
    return m_life;
}

int Unit::maxLife() const
{
    return m_maxLife;
}

int Unit::lifeAnim() const
{
    return m_lifeAnim;
}

void Unit::updateLifeAnim()
{
    if(m_lifeAnim > m_life) m_lifeAnim --;
    if(m_lifeAnim < m_life) m_lifeAnim ++;

    if((m_lifeAnim == 0)&&(!m_isResurecting))
    {
        QTimer::singleShot(m_deathDuration, this, SLOT(resurrect()));
        m_isResurecting = true;
        m_invulnerable = true;
    }
    if(m_lifeAnim == m_maxLife) m_dead = false;
}

bool Unit::dead() const
{
    return m_dead;
}

void Unit::setDead(bool dead)
{
    m_dead = dead;
}

void Unit::takeDamage(int damage)
{
    if(m_invulnerable) return;
    m_life -= damage;
    if(m_life <= 0)
    {
        m_life = 0;
        setDead(true);
    }
}

bool Unit::isUnit() const
{
    return true;
}

void Unit::resurrect()
{
    m_life = m_maxLife;
    m_deathDuration += 1000;
    m_isResurecting = false;
    QTimer::singleShot(m_invulnerabilityDuration, this, SLOT(removeInvulnerability()));
}

void Unit::removeInvulnerability()
{
    m_invulnerable = false;
}
bool Unit::invulnerable() const
{
    return m_invulnerable;
}

void Unit::setInvulnerable(bool invulnerable)
{
    m_invulnerable = invulnerable;
}





