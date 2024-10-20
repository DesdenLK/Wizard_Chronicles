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
	HELLO_LEFT, HELLO_RIGHT, STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, STOPPING_LEFT, STOPPING_RIGHT, 
		JUMP_LEFT, JUMP_RIGHT, FALL_LEFT, FALL_RIGHT, CROUCH_LEFT, CROUCH_RIGHT, CLIMB, NUM_ANIMS
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	Jumping = false;
	loopTimesInactive = 0;
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
	
	//HELLO_LEFT
	sprite->setAnimationSpeed(HELLO_LEFT, 8);
	sprite->addKeyframe(HELLO_LEFT, glm::vec2(0.f, 3 * SPRITE_HEIGHT));
	sprite->addKeyframe(HELLO_LEFT, glm::vec2(0.f, 4 * SPRITE_HEIGHT));
	sprite->addKeyframe(HELLO_LEFT, glm::vec2(0.f, 5 * SPRITE_HEIGHT));
	sprite->addKeyframe(HELLO_LEFT, glm::vec2(0.f, 6 * SPRITE_HEIGHT));

	//HELLO_RIGHT
	sprite->setAnimationSpeed(HELLO_RIGHT, 8);
	sprite->addKeyframe(HELLO_RIGHT, glm::vec2(0.f, 3 * SPRITE_HEIGHT));
	sprite->addKeyframe(HELLO_RIGHT, glm::vec2(0.f, 4 * SPRITE_HEIGHT));
	sprite->addKeyframe(HELLO_RIGHT, glm::vec2(0.f, 5 * SPRITE_HEIGHT));
	sprite->addKeyframe(HELLO_RIGHT, glm::vec2(0.f, 6 * SPRITE_HEIGHT));

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

	//JUMP_LEFT
	sprite->setAnimationSpeed(JUMP_LEFT, 4);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(6 * SPRITE_WIDTH, 2 * SPRITE_HEIGHT));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(6 * SPRITE_WIDTH, 3 * SPRITE_HEIGHT));

	//JUMP_RIGHT
	sprite->setAnimationSpeed(JUMP_RIGHT, 4);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(5 * SPRITE_WIDTH, 2 * SPRITE_HEIGHT));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(5 * SPRITE_WIDTH, 3 * SPRITE_HEIGHT));

	//CROUCH_LEFT
	sprite->setAnimationSpeed(CROUCH_LEFT, 8);
	sprite->addKeyframe(CROUCH_LEFT, glm::vec2(6 * SPRITE_WIDTH, 0.f));
	sprite->addKeyframe(CROUCH_LEFT, glm::vec2(6 * SPRITE_WIDTH, SPRITE_HEIGHT));

	//CROUCH_RIGHT
	sprite->setAnimationSpeed(CROUCH_RIGHT, 8);
	sprite->addKeyframe(CROUCH_RIGHT, glm::vec2(5 * SPRITE_WIDTH, 0.f));
	sprite->addKeyframe(CROUCH_RIGHT, glm::vec2(5 * SPRITE_WIDTH, SPRITE_HEIGHT));

	//FALL_LEFT
	sprite->setAnimationSpeed(FALL_LEFT, 8);
	sprite->addKeyframe(FALL_LEFT, glm::vec2(6 * SPRITE_WIDTH, 6 * SPRITE_HEIGHT));
	sprite->addKeyframe(FALL_LEFT, glm::vec2(6 * SPRITE_WIDTH, 7 * SPRITE_HEIGHT));

	//FALL_RIGHT
	sprite->setAnimationSpeed(FALL_RIGHT, 8);
	sprite->addKeyframe(FALL_RIGHT, glm::vec2(5 * SPRITE_WIDTH, 6 * SPRITE_HEIGHT));
	sprite->addKeyframe(FALL_RIGHT, glm::vec2(5 * SPRITE_WIDTH, 7 * SPRITE_HEIGHT));

	//CLIMB
	sprite->setAnimationSpeed(CLIMB, 4);
	sprite->addKeyframe(CLIMB, glm::vec2(4 * SPRITE_WIDTH, 0.0f));
	sprite->addKeyframe(CLIMB, glm::vec2(4 * SPRITE_WIDTH, SPRITE_HEIGHT));
	sprite->addKeyframe(CLIMB, glm::vec2(4 * SPRITE_WIDTH, 2*SPRITE_HEIGHT));

	sprite->changeAnimation(2);
}

void Player::updatePlayerMovement(int deltaTime) {
	bool ladderCollision = map->objectCollision(posPlayer, glm::vec2(32, 32));
	if (!ladderCollision) {
		Climbing = false;
		isOnLadderTop = false;
	}
	if (Game::instance().getKey(GLFW_KEY_A)) playerKey_A(deltaTime);

	else if (Game::instance().getKey(GLFW_KEY_D)) playerKey_D(deltaTime);

	else playerNOKeys(deltaTime);
	
	if (Game::instance().getKey(GLFW_KEY_S)) playerKey_S(deltaTime);

	if (Game::instance().getKey(GLFW_KEY_W) && ladderCollision || Jumping) playerKey_W(deltaTime);
	else if (!ladderCollision) playerFalling(deltaTime);
}

void Player::playerKey_A(int deltaTime) {
	playerAcceleration.x = 0.01f;
	playerVelocity.x = min(2.f, playerVelocity.x + playerAcceleration.x * deltaTime);
	//cout << "A: " << playerVelocity.x << endl;
	glm::vec2 initialPosPlayer = posPlayer;
	posPlayer.x -= playerVelocity.x;

	if (!Jumping) {
		if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
	}

	// Poisicio final
	glm::vec2 targetPosPlayer = posPlayer;

	// Mirar colisions en el path del jugador
	for (float t = 0.0f; t <= 1.0f; t += 1.0f / playerVelocity.x) {
		glm::vec2 interpolatedPos = initialPosPlayer + t * (targetPosPlayer - initialPosPlayer);
		if (map->collisionMoveLeft(interpolatedPos, glm::ivec2(32, 32))) {
			posPlayer.x = initialPosPlayer.x;
			sprite->changeAnimation(STAND_LEFT);
			break;
		}
	}
}


void Player::playerKey_D(int deltaTime) {
	playerAcceleration.x = 0.01f;
	playerVelocity.x = min(2.f, playerVelocity.x + playerAcceleration.x * deltaTime);
	//cout << "D: " << playerVelocity.x << endl;
	glm::vec2 initialPosPlayer = posPlayer;
	posPlayer.x += playerVelocity.x;

	if (!Jumping) {
		if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
	}
	// Poisicio final
	glm::vec2 targetPosPlayer = posPlayer;

	// Mirar colisions en el path del jugador
	for (float t = 0.0f; t <= 1.0f; t += 1.0f / playerVelocity.x) {
		glm::vec2 interpolatedPos = initialPosPlayer + t * (targetPosPlayer - initialPosPlayer);
		if (map->collisionMoveRight(interpolatedPos, glm::ivec2(32, 32))) {
			posPlayer.x = initialPosPlayer.x;
			sprite->changeAnimation(STAND_RIGHT);
			break;
		}
	}
}

void Player::playerNOKeys(int deltaTime) {
	switch (sprite->animation()) {

		case STAND_LEFT:
			if (loopTimesInactive >= 500) { sprite->changeAnimation(HELLO_LEFT); loopTimesInactive = 0; }
			else ++loopTimesInactive;
			cout << "Loop: " << loopTimesInactive << endl;
			break;
		
		case STAND_RIGHT:
			if (loopTimesInactive >= 500) { sprite->changeAnimation(HELLO_RIGHT); loopTimesInactive = 0; }
			else ++loopTimesInactive;
			break;

		case MOVE_LEFT:
			sprite->changeAnimation(STOPPING_LEFT);
			break;

		case MOVE_RIGHT:
			sprite->changeAnimation(STOPPING_RIGHT);
			break;

		case STOPPING_LEFT:
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
			break;

		case STOPPING_RIGHT:
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
			break;

		case JUMP_LEFT:
			sprite->changeAnimation(STAND_LEFT);
			break;

		case JUMP_RIGHT:
			sprite->changeAnimation(STAND_RIGHT);
			break;

		default:
			break;
	}
}

void Player::playerFalling(int deltaTime)
{
	posPlayer.y += FALL_STEP;
	playerVelocity.y = FALL_STEP;
	if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
	{
		if (sprite->animation() == FALL_LEFT) sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == FALL_RIGHT) sprite->changeAnimation(STAND_RIGHT);

		playerVelocity.y = 0;
		if (Game::instance().getKey(GLFW_KEY_W))
		{
			Jumping = true;
			jumpAngle = 0;
			startY = int(posPlayer.y);
		}
	}
}

void Player::playerKey_W(int deltaTime) {
	// fet per l'escala, adaptar metodes per quan tinguem objectes que no es poden traspassar
	if (map->objectCollision(posPlayer, glm::vec2(32, 32))) {
		Climbing = true;
		if (!isOnLadderTop) {
			playerVelocity.y = 0.1 * deltaTime;
			posPlayer.y -= playerVelocity.y;
			if (sprite->animation() != CLIMB) sprite->changeAnimation(CLIMB);
			if (map->isOnLadderTop(posPlayer, glm::vec2(32, 32))) {
				isOnLadderTop = true;
				playerVelocity.y = 0;
				cout << "on top of the ladder" << endl;
			}
		}
	}
	else {
		Climbing = false;
		isOnLadderTop = false;
		switch (sprite->animation()) {
		case MOVE_LEFT:
			sprite->changeAnimation(JUMP_LEFT);
			break;

		case MOVE_RIGHT:
			sprite->changeAnimation(JUMP_RIGHT);
			break;

		case STAND_LEFT:
			sprite->changeAnimation(JUMP_LEFT);
			break;

		case STAND_RIGHT:
			sprite->changeAnimation(JUMP_RIGHT);
			break;

		case CROUCH_LEFT:
			sprite->changeAnimation(JUMP_LEFT);
			break;

		case CROUCH_RIGHT:
			sprite->changeAnimation(JUMP_RIGHT);
			break;
		
		case CLIMB:
			sprite->changeAnimation(STAND_RIGHT);
			break;
		}
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
}

void Player::playerKey_S(int deltaTime)
{
	if (isOnLadderTop) {
		Climbing = true;
		isOnLadderTop = false;
		Jumping = false;
	}
	if (Climbing) {
		playerVelocity.y = 0.1f * deltaTime;
		posPlayer.y += playerVelocity.y;

		if (sprite->animation() != CLIMB) {
			sprite->changeAnimation(CLIMB);
		}
		if (map->isOnLadderBottom(posPlayer, glm::vec2(32, 32))) {
			Climbing = false;
			playerVelocity.y = 0;
			cout << "on BOTTOM of the ladder" << endl;
		}
	}

	else if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT || sprite->animation() == HELLO_LEFT) sprite->changeAnimation(CROUCH_LEFT);
	else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT || sprite->animation() == HELLO_RIGHT) sprite->changeAnimation(CROUCH_RIGHT);

	else if (sprite->animation() == JUMP_LEFT) sprite->changeAnimation(FALL_LEFT);
	else if (sprite->animation() == JUMP_RIGHT) sprite->changeAnimation(FALL_RIGHT);
}



