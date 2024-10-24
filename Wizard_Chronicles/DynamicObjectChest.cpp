#include "DynamicObjectChest.h"

#include <iostream>
#include "TileMap.h"



enum BoxAnims { CHEST, DESTROY_CHEST, OPEN_CHEST, NUM_ANIMS };


void DynamicObjectChest::init(int id, string pathToFile, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, glm::vec2 offSet, ShaderProgram& shaderProgram, TileMap* map)
{
	DynamicObject::init(id, pathToFile, x, y, w, h, quadSize, spriteWidth, spriteHeight, offSet, shaderProgram, map);
	setAnimations();
}


void DynamicObjectChest::setAnimations()
{
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(CHEST, 8);
	sprite->addKeyframe(CHEST, glm::vec2(0, 0));

	sprite->setAnimationSpeed(DESTROY_CHEST, 8);

	sprite->addKeyframe(DESTROY_CHEST, glm::vec2(4*0.125, 0));
	sprite->addKeyframe(DESTROY_CHEST, glm::vec2(5 * 0.125, 0));
	sprite->addKeyframe(DESTROY_CHEST, glm::vec2(6 * 0.125, 0));
	sprite->addKeyframe(DESTROY_CHEST, glm::vec2(7 * 0.125, 0));

	sprite->setAnimationSpeed(OPEN_CHEST, 8);
	sprite->addKeyframe(OPEN_CHEST, glm::vec2(0.125, 0));
	sprite->addKeyframe(OPEN_CHEST, glm::vec2(2 *0.125, 0));
	sprite->addKeyframe(OPEN_CHEST, glm::vec2(3 * 0.125, 0));


	sprite->changeAnimation(0);

}

void DynamicObjectChest::objectFalling()
{
	posicio.y += FALL_STEP;
	speed.y = FALL_STEP;

	if (speed.x > 0 and map->collisionMoveRight(posicio, measures)) { speed.x = 0; openChest(); }
	else if (speed.x < 0 and map->collisionMoveLeft(posicio, measures)) { speed.x = 0; openChest(); }

	posicio.x += speed.x;
	if (map->collisionMoveDown(posicio, measures, &posicio.y))
	{
		//cout << "collision detected" << endl;
		speed.x = 0;
		speed.y = 0;
		destroyObject();
	}
}

void DynamicObjectChest::objectJump()
{
	speed.y = JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f);
	jumpAngle += JUMP_ANGLE_STEP;

	if (speed.x > 0 and map->collisionMoveRight(posicio, measures)) { speed.x = 0; openChest(); }
	else if (speed.x < 0 and map->collisionMoveLeft(posicio, measures)) { speed.x = 0; openChest(); }
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

void DynamicObjectChest::destroyObject()
{
	if (objectState.objectThrowed and !objectState.destroyed)
	{
		objectState.destroyed = true;
		objectState.hitboxEnabled = false;
		sprite->changeAnimation(DESTROY_CHEST);

		map->destroyDynamicObject(id);
	}
	else objectState.objectThrowed = false;

}

void DynamicObjectChest::openChest()
{
	sprite->changeAnimation(OPEN_CHEST);
	sprite->update(1000);
	sprite->render();
	sprite->update(1000);
	sprite->render();
	sprite->update(1000);
	sprite->render();

	//destroyObject();
}


