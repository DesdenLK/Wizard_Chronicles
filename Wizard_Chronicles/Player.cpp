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
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, STOPPING_LEFT, STOPPING_RIGHT, JUMP_LEFT, JUMP_RIGHT, FALL_LEFT, FALL_RIGHT, JUMP, FALL, NUM_ANIMS
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	Jumping, Falling = false;
	playerVelocity = glm::vec2(0, 0);
	playerAcceleration = glm::vec2(0, 0);


	spritesheet.loadFromFile("images/Mickey.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(SPRITE_WIDTH, SPRITE_HEIGHT), &spritesheet, &shaderProgram);
	
	setAnimations();

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	updatePlayerMovement(deltaTime);
	
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
	sprite->setNumberAnimations(NUM_ANIMS);
	
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

	//STOPPING_LEFT
	sprite->setAnimationSpeed(STOPPING_LEFT, 1);
	sprite->addKeyframe(STOPPING_LEFT, glm::vec2(SPRITE_WIDTH, 2 * SPRITE_HEIGHT));

	//STOPPING_RIGHT
	sprite->setAnimationSpeed(STOPPING_RIGHT, 1);
	sprite->addKeyframe(STOPPING_RIGHT, glm::vec2(0.f, 2 * SPRITE_HEIGHT));


	sprite->changeAnimation(0);
}

void Player::updatePlayerMovement(int deltaTime) {

	if (Game::instance().getKey(GLFW_KEY_A)) playerKey_A(deltaTime);

	else if (Game::instance().getKey(GLFW_KEY_D)) playerKey_D(deltaTime);

	else playerNOKeys(deltaTime);

	if (Jumping)
	{
		playerVelocity.y = 96 * sin(3.14159f * jumpAngle / 180.f);
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			Jumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = startY - 96 * sin(3.14159f * jumpAngle / 180.f);
			if (jumpAngle > 90)
				Jumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
			if (jumpAngle <= 90) {	// player going up
				Jumping = !map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
			}
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		playerVelocity.y = FALL_STEP;
		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			playerVelocity.y = 0;
			if (Game::instance().getKey(GLFW_KEY_UP))
			{
				Jumping = true;
				jumpAngle = 0;
				startY = int(posPlayer.y);
			}
		}
	}
}

void Player::playerKey_A(int deltaTime) {
	playerAcceleration.x = 0.01f;
	playerVelocity.x = min(2.f, playerVelocity.x + playerAcceleration.x * deltaTime);
	//cout << "A: " << playerVelocity.x << endl;
	posPlayer.x -= playerVelocity.x;

	if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
	if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
	{
		posPlayer.x += playerVelocity.x;
		sprite->changeAnimation(STAND_LEFT);
	}
}


void Player::playerKey_D(int deltaTime) {
	playerAcceleration.x = 0.01f;
	playerVelocity.x = min(2.f, playerVelocity.x + playerAcceleration.x * deltaTime);
	//cout << "D: " << playerVelocity.x << endl;
	posPlayer.x += playerVelocity.x;

	if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
	if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
	{
		posPlayer.x -= playerVelocity.x;
		sprite->changeAnimation(STAND_RIGHT);
	}
}

void Player::playerNOKeys(int deltaTime) {

	if (sprite->animation() == MOVE_LEFT) sprite->changeAnimation(STOPPING_LEFT);

	else if (sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(STOPPING_RIGHT);

	else if (sprite->animation() == STOPPING_LEFT) {
		if (playerVelocity.x == 0) sprite->changeAnimation(STAND_LEFT);
		else playerVelocity.x = max(0.f, playerVelocity.x - playerAcceleration.x * deltaTime);
		cout << "Vel: " << playerVelocity.x << endl;

		if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += playerVelocity.x;
			sprite->changeAnimation(STAND_LEFT);
			playerVelocity.x = 0;
		}

		else posPlayer.x -= playerVelocity.x;

	}
	else if (sprite->animation() == STOPPING_RIGHT) {
		if (playerVelocity.x == 0) sprite->changeAnimation(STAND_RIGHT);
		else playerVelocity.x = max(0.f, playerVelocity.x - playerAcceleration.x * deltaTime);
		cout << "Vel: " << playerVelocity.x << endl;

		if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= playerVelocity.x;
			sprite->changeAnimation(STAND_RIGHT);
			playerVelocity.x = 0;
		}

		else posPlayer.x += playerVelocity.x;
	}
}



