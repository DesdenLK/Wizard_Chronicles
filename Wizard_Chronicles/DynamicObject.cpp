#include "DynamicObject.h"
#include <iostream>
#include "TileMap.h"


/*
void DynamicObject::init(string pathToFile, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, glm::vec2 offSet, ShaderProgram& shaderProgram, TileMap *map)
{
	posicio = glm::vec2(x, y);
	measures = glm::vec2(w, h);
	this -> spriteOffset = offSet;
	this->map = map;


	objectState = { false, false };

	speed = glm::vec2(0, 0);

	spritesheet.loadFromFile(pathToFile, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(quadSize, glm::vec2(spriteWidth, spriteHeight), &spritesheet, &shaderProgram);

	sprite->setPosition(posicio);

}
*/

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
}

void DynamicObject::dropObject(float XSpeed)
{
	objectState.pickedUp = false;
	objectState.Jumping = true;
	jumpAngle = 0;
	startY = posicio.y;
	speed.x = XSpeed;
}

void DynamicObject::objectFalling()
{
	posicio.y += FALL_STEP;
	speed.y = FALL_STEP;

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

bool DynamicObject::IsPickedUp() const
{
	return objectState.pickedUp;
}

void DynamicObject::setSpeed(glm::vec2 speed)
{
	this->speed = speed;
}
