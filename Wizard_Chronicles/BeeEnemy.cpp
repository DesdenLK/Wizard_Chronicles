#include "BeeEnemy.h"
#include "TileMap.h"
#include "iostream"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define FALL_VELOCITY_DEAD 3

#define SPRITE_HEIGHT 1.f
#define SPRITE_WIDTH (1.f/8.f)

enum EnemyAnims
{
	FLY_RIGHT, FLY_LEFT, DIE_RIGHT, DIE_LEFT, NUM_ANIMS
};

void BeeEnemy::updateEnemyMovement(int deltaTime) {
	posEnemy += EnemyVelocity;
	if ((EnemyVelocity.x < 0 and (map->collisionMoveLeft(posEnemy, glm::ivec2(25, 25))) or 
		(EnemyVelocity.x > 0 and map->collisionMoveRight(posEnemy, glm::ivec2(25, 25)))) or
		map->collisionWithInvisibleObject(posEnemy,glm::ivec2(25,25))) {
		posEnemy -= EnemyVelocity;
		EnemyVelocity = -EnemyVelocity;
		switch (sprite->animation()) {
		case FLY_LEFT:
			sprite->changeAnimation(true, FLY_RIGHT);
			break;
		case FLY_RIGHT:
			sprite->changeAnimation(true, FLY_LEFT);
			break;
		default:
			break;
		}
	}
}

void BeeEnemy::setAnimations() {
	sprite->setNumberAnimations(NUM_ANIMS);

	//FLY_LEFT
	sprite->setAnimationSpeed(FLY_LEFT, 4);
	sprite->addKeyframe(FLY_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(SPRITE_WIDTH, 0.f));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(2 * SPRITE_WIDTH, 0.f));

	//FLY_RIGHT
	sprite->setAnimationSpeed(FLY_RIGHT, 4);
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(3 * SPRITE_WIDTH, 0.f));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(4 * SPRITE_WIDTH, 0.f));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(5 * SPRITE_WIDTH, 0.f));

	//DIE_RIGHT
	sprite->setAnimationSpeed(DIE_RIGHT, 4);
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(6 * SPRITE_WIDTH, 0.f));

	//DIE_LEFT
	sprite->setAnimationSpeed(DIE_LEFT, 4);
	sprite->addKeyframe(DIE_LEFT, glm::vec2(7 * SPRITE_WIDTH, 0.f));

	sprite->changeAnimation(true, FLY_LEFT);

}

void BeeEnemy::changeToDeadAnimation() {
	if (EnemyVelocity.x < 0) sprite->changeAnimation(true, DIE_LEFT);
	else sprite->changeAnimation(true, DIE_RIGHT);
	//EnemyVelocity = glm::vec2(0.f, (496 - posEnemy.y) / (eraseAnimationTime/60));
	EnemyVelocity = glm::vec2(0.f, FALL_VELOCITY_DEAD);
	//cout << "pos y: " << posEnemy.y << " velocity y: " << EnemyVelocity.y << endl;
}

int BeeEnemy::getEraseAnimationTime() {
	return eraseAnimationTime;
}