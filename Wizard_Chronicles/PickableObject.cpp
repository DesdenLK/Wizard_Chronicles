#include "PickableObject.h"
#include "TileMap.h"



void PickableObject::render()
{
	sprite->render();
}

void PickableObject::update(int deltaTime)
{
	if (objectState.Jumping) {
		objectJump();
	}
	else objectFalling();


	sprite->setPosition(posicio);
	sprite->update(deltaTime);
}

void PickableObject::objectFalling()
{
	posicio.y += FALL_STEP_PICKOBJ;
	speed.y = FALL_STEP_PICKOBJ;

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

void PickableObject::objectJump()
{
	speed.y = JUMP_HEIGHT_PICKOBJ * sin(3.14159f * jumpAngle / 180.f);
	jumpAngle += JUMP_ANGLE_STEP_PICKOBJ;

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


bool PickableObject::isPickable()
{
	return true;
}
