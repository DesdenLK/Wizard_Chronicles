#include "DynamicObject.h"
#include <iostream>
#include "TileMap.h"



void DynamicObject::init(int id, string pathToFile, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, glm::vec2 offSet, ShaderProgram& shaderProgram, TileMap *map)
{
	this->id = id;
	posicio = glm::vec2(x, y);
	measures = glm::vec2(w, h);
	this -> spriteOffset = offSet;
	this->map = map;


	objectState = { false, false, false, false, true };
	loopTimesDestroy = 0;

	speed = glm::vec2(0, 0);

	spritesheet.loadFromFile(pathToFile, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(quadSize, glm::vec2(spriteWidth, spriteHeight), &spritesheet, &shaderProgram);

	sprite->setPosition(posicio);

}


void DynamicObject::update(int deltaTime)
{
	if (objectState.Jumping) {
		objectJump();
	}
	else objectFalling();



	
	sprite->update(deltaTime);
	sprite->setPosition(posicio);
	//cout << "X: " << posicio.x << " Y: " << posicio.y << endl;
}

void DynamicObject::destroyObject()
{
	return;
}

void DynamicObject::render()
{
	sprite->render();
}

glm::vec2 DynamicObject::getSpeed() const
{
	return speed;
}

void DynamicObject::pickObject()
{
	objectState.pickedUp = true;
	objectState.hitboxEnabled = false;
}

void DynamicObject::dropObject(float XSpeed)
{
	objectState.pickedUp = false;
	objectState.hitboxEnabled = true;
	objectState.objectThrowed = true;
	objectState.Jumping = true;
	jumpAngle = 0;
	startY = posicio.y;
	speed.x = XSpeed;
}

void DynamicObject::openChest(bool playerDropping)
{
	return;
}

void DynamicObject::objectFalling()
{
	posicio.y += FALL_STEP;
	speed.y = FALL_STEP;

	int enemyCollidedId = map->enemyCollision(posicio, measures);
	if (enemyCollidedId != -1 and objectState.hitboxEnabled) map->eraseEnemy(enemyCollidedId);
	if (speed.x > 0 and map->collisionMoveRight(posicio, measures)) speed.x = 0;
	else if (speed.x < 0 and map->collisionMoveLeft(posicio, measures)) speed.x = 0;

	posicio.x += speed.x;
	if (map->collisionMoveDown(posicio, measures, &posicio.y))
	{
		//cout << "collision detected" << endl;
		speed.x = 0;
		speed.y = 0;
	}
}

void DynamicObject::objectJump()
{
	speed.y = JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f);
	jumpAngle += JUMP_ANGLE_STEP;

	int enemyCollidedId = map->enemyCollision(posicio, measures);
	if (enemyCollidedId != -1 and objectState.hitboxEnabled) map->eraseEnemy(enemyCollidedId);

	if (speed.x > 0 and map->collisionMoveRight(posicio, measures)) speed.x = 0;
	else if (speed.x < 0 and map->collisionMoveLeft(posicio, measures)) speed.x = 0;
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

void DynamicObject::setPickableObject(string item)
{
	return;
}

bool DynamicObject::IsPickedUp() const
{
	return objectState.pickedUp;
}

bool DynamicObject::IsDestroyed() const
{
	return objectState.destroyed;
}

bool DynamicObject::IsHitboxEnabled() const
{
	return objectState.hitboxEnabled;
}

bool DynamicObject::isChest() const
{
	return false;
}

void DynamicObject::setSpeed(glm::vec2 speed)
{
	this->speed = speed;
}
