#include "unit.h"

Unit::Unit()
{
    m_position.x = 0;
    m_position.y = 0;

    m_maxLife = 100;
    m_life = m_maxLife;
    m_lifeAnim = m_maxLife;
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

    if(m_lifeAnim == 0) m_life = m_maxLife;
}

void Unit::takeDamage(int damage)
{
    m_life -= damage;
    if(m_life < 0)
    {
        m_life = 0;
        m_dead = true;
    }
}

bool Unit::isUnit() const
{
    return true;
}




