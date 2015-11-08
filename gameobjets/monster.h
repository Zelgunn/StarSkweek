#ifndef MONSTER_H
#define MONSTER_H

#include "unit.h"

class Monster : public Unit
{
public:
    Monster();

    bool isMonster() const;
};

#endif // MONSTER_H
