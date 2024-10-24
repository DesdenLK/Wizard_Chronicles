#ifndef _CaterpillarEnemy_INCLUDE
#define _CaterpillarEnemy_INCLUDE

#include "Enemy.h"

class CaterpillarEnemy :
    public Enemy
{
public:
    void updateEnemyMovement(int deltaTime) override;
    void setAnimations() override;
    void changeToDeadAnimation() override;
    int getEraseAnimationTime() override;
};

#endif
