#ifndef _BeeEnemy_INCLUDE
#define _BeeEnemy_INCLUDE

#include "Enemy.h"

class BeeEnemy :
    public Enemy
{
public:
    void updateEnemyMovement(int deltaTime) override;
    void setAnimations() override;
    void changeToDeadAnimation() override;
    int getEraseAnimationTime() override;
};

#endif
