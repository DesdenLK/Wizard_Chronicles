#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define SPRITE_WIDTH 1/20.f
#define SPRITE_HEIGHT 1/16.f


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT, FALL_LEFT, FALL_RIGHT, JUMP, FALL
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	playerVelocity = glm::vec2(0, 0);
	spritesheet.loadFromFile("images/Mickey.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(SPRITE_WIDTH, SPRITE_HEIGHT), &spritesheet, &shaderProgram);
	
	setAnimations();

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	playerVelocity.x = 2;
	if(Game::instance().getKey(GLFW_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= playerVelocity.x;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += playerVelocity.x;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += playerVelocity.x;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= playerVelocity.x;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else
	{
		if (sprite->animation() == MOVE_LEFT) {
			sprite->changeAnimation(STAND_LEFT);
			playerVelocity.x = 0;
		}
			
		else if (sprite->animation() == MOVE_RIGHT) {
			sprite->changeAnimation(STAND_RIGHT);
			playerVelocity.x = 0;
		}
	}
	
	if(bJumping)
	{
		playerVelocity.y = 96 * sin(3.14159f * jumpAngle / 180.f);
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
			if (jumpAngle <= 90) {	// player going up
				bJumping = !map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
			}
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		playerVelocity.y = FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			playerVelocity.y = 0;
			if(Game::instance().getKey(GLFW_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

glm::vec2 Player::getPosition() {
	return posPlayer;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Player::getVelocity() {
	return playerVelocity;
}

void Player::setAnimations() {
	sprite->setNumberAnimations(4);
	
	//STAND_LEFT
	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(SPRITE_WIDTH, 0.f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(SPRITE_WIDTH, SPRITE_HEIGHT));

	//STAND_RIGHT
	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, SPRITE_HEIGHT));

	//MOVE_LEFT
	sprite->setAnimationSpeed(MOVE_LEFT, 16);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * SPRITE_WIDTH, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * SPRITE_WIDTH, SPRITE_HEIGHT));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * SPRITE_WIDTH, 2 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * SPRITE_WIDTH, 3 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * SPRITE_WIDTH, 4 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * SPRITE_WIDTH, 5 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * SPRITE_WIDTH, 6 * SPRITE_HEIGHT));

	//MOVE_RIGHT
	sprite->setAnimationSpeed(MOVE_RIGHT, 16);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * SPRITE_WIDTH, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * SPRITE_WIDTH, SPRITE_HEIGHT));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * SPRITE_WIDTH, 2 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * SPRITE_WIDTH, 3 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * SPRITE_WIDTH, 4 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * SPRITE_WIDTH, 5 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * SPRITE_WIDTH, 6 * SPRITE_HEIGHT));


	sprite->changeAnimation(0);
}



