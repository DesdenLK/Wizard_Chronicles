#include "CaterpillarEnemy.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define SPRITE_HEIGHT 1.f
#define SPRITE_WIDTH (1.f/6.f)

enum EnemyAnims
{
	WALK_LEFT, WALK_RIGHT, DIE_RIGHT, DIE_LEFT, NUM_ANIMS
};

void CaterpillarEnemy::updateEnemyMovement(int deltaTime) {
	return;
}

void CaterpillarEnemy::setAnimations() {
	sprite->setNumberAnimations(NUM_ANIMS);

	//WALK_LEFT
	sprite->setAnimationSpeed(WALK_LEFT, 4);
	sprite->addKeyframe(WALK_LEFT, glm::vec2(5*SPRITE_WIDTH, 0.0f));
	sprite->addKeyframe(WALK_LEFT, glm::vec2(3*SPRITE_WIDTH, 0.0f));

	//WALK_RIGHT
	sprite->setAnimationSpeed(WALK_RIGHT, 4);
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(4*SPRITE_WIDTH, 0.0f));

	//SLIDE_LEFT
	sprite->setAnimationSpeed(DIE_LEFT, 4);
	sprite->addKeyframe(DIE_LEFT, glm::vec2(2*SPRITE_WIDTH, 0.0f));

	//SLIDE_RIGHT
	sprite->setAnimationSpeed(DIE_RIGHT, 4);
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(SPRITE_WIDTH, 0.0f));
	
	sprite->changeAnimation(WALK_LEFT);

}
