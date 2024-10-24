#include "DynamicObjectBox.h"
#include <iostream>
#include "TileMap.h"



enum BoxAnims {BOX, DESTROY_BOX, NUM_ANIMS};


void DynamicObjectBox::init(int id, string pathToFile, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, glm::vec2 offSet, ShaderProgram& shaderProgram, TileMap* map)
{
	DynamicObject::init(id, pathToFile, x, y, w, h, quadSize, spriteWidth, spriteHeight, offSet, shaderProgram, map);
	setAnimations();
}


void DynamicObjectBox::setAnimations()
{
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(BOX, 8);
	sprite->addKeyframe(BOX, glm::vec2(0, 0));

	sprite->setAnimationSpeed(DESTROY_BOX, 8);

	sprite->addKeyframe(DESTROY_BOX, glm::vec2(0.2, 0));
	sprite->addKeyframe(DESTROY_BOX, glm::vec2(0.4, 0));
	sprite->addKeyframe(DESTROY_BOX, glm::vec2(0.6, 0));
	sprite->addKeyframe(DESTROY_BOX, glm::vec2(0.8, 0));

	sprite->changeAnimation(0);

}

void DynamicObjectBox::objectFalling()
{
	posicio.y += FALL_STEP;
	speed.y = FALL_STEP;

	int enemyCollidedId = map->enemyCollision(posicio, measures);
	if (enemyCollidedId != -1) {
		map->eraseEnemy(enemyCollidedId);
		destroyObject();
	}
	else {
		if (speed.x > 0 and map->collisionMoveRight(posicio, measures)) {
			speed.x = 0;
			destroyObject();
		}
		else if (speed.x < 0 and map->collisionMoveLeft(posicio, measures)) {
			speed.x = 0;
			destroyObject();
		}

		posicio.x += speed.x;
		if (map->collisionMoveDown(posicio, measures, &posicio.y))
		{
			//cout << "collision detected" << endl;
			speed.x = 0;
			speed.y = 0;
			destroyObject();
		}
	}
	
}

void DynamicObjectBox::objectJump()
{
	speed.y = JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f);
	jumpAngle += JUMP_ANGLE_STEP;

	int enemyCollidedId = map->enemyCollision(posicio, measures);
	if (enemyCollidedId != -1) {
		map->eraseEnemy(enemyCollidedId);
		destroyObject();
	}
	else {
		if (speed.x > 0 and map->collisionMoveRight(posicio, measures)) { speed.x = 0; destroyObject(); }
		else if (speed.x < 0 and map->collisionMoveLeft(posicio, measures)) { speed.x = 0; destroyObject(); }
		posicio.x += speed.x;

		if (jumpAngle == 180)
		{
			objectState.Jumping = false;
			posicio.y = startY;
		}
		else
		{
			posicio.y = startY - speed.y;
			if (jumpAngle > 90)
				objectState.Jumping = (!map->collisionMoveDown(posicio, measures, &posicio.y));
			if (jumpAngle <= 90) {	// player going up
				objectState.Jumping = !map->collisionMoveUp(posicio, measures, &posicio.y);
			}
		}
	}
}

void DynamicObjectBox::destroyObject()
{
	if (objectState.objectThrowed and !objectState.destroyed)
	{
		objectState.destroyed = true;
		objectState.hitboxEnabled = false;
		sprite->changeAnimation(DESTROY_BOX);

		map->destroyDynamicObject(id);
	}
	else objectState.objectThrowed = false;

}


