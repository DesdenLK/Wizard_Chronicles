#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"
#include "TileMap.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 60
#define FALL_STEP 4

#define SPRITE_WIDTH 1/20.f
#define SPRITE_HEIGHT 1/16.f

#define HURT_TIME 1000 //in ms
#define HURT_FRAME_TIME 50
#define VERTICAL_COL_TIMEOUT 0 //in ms


enum PlayerAnims
{
	HELLO_LEFT, HELLO_RIGHT, STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, STOPPING_LEFT, STOPPING_RIGHT, 
	JUMP_LEFT, JUMP_RIGHT, FALL_LEFT, FALL_RIGHT, CROUCH_LEFT, CROUCH_RIGHT, CLIMB, PICKING_LEFT, PICKING_RIGHT,
	STAND_LEFT_OBJECT,STAND_RIGHT_OBJECT,
	MOVING_LEFT_OBJECT, MOVING_RIGHT_OBJECT, NUM_ANIMS
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	playerState = { false, false, false, false};
	loopTimesInactive = 0;
	verticalCollisionTimeout = 0;
	playerVelocity = glm::vec2(0, 0);
	playerAcceleration = glm::vec2(0, 0);

	objectPickedUp = nullptr;

	spritesheet.loadFromFile("images/Mickey.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(SPRITE_WIDTH, SPRITE_HEIGHT), &spritesheet, &shaderProgram);
	
	setAnimations();

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	playerLifes = 3;
	playerScore = 0;
	godMode = false;
	gPressed = false;
	
}

void Player::update(int deltaTime)
{

	if (Game::instance().getKey(GLFW_KEY_G) and not gPressed) gPressed = true;
	else if (not Game::instance().getKey(GLFW_KEY_G) and gPressed) {
		gPressed = false;
		godMode = not godMode;
	}


	if (Game::instance().getKey(GLFW_KEY_H)) playerLifes = 3;

	if (isHurt) {
		if (hurtTime == HURT_TIME and not godMode) --playerLifes;
		hurtTime -= deltaTime;
		hurtFrameTime -= deltaTime;
		if (hurtTime <= 0) {
			sprite->setAlpha(1.f);
			isHurt = false;
		}
		else if (hurtFrameTime <= 0) {
			if (sprite->getAlpha() == 0.3) sprite->setAlpha(0.6);
			else sprite->setAlpha(0.3);
			hurtFrameTime = HURT_FRAME_TIME;
		}
	}

	sprite->update(deltaTime);
	updatePlayerMovement(deltaTime);

	int pickableObjectID = map->collisionWithPickableObject(posPlayer, glm::vec2(32, 32));
	if (pickableObjectID != -1) {
		string type = map->getPickableObjectType(pickableObjectID);
		if (type == "Cake") { map->setPlayerScore(map->getPlayerScore() + 10); playerLifes = min(3, playerLifes + 1); }
		else if (type == "Coin") { map->setPlayerScore(map->getPlayerScore() + 500); }
		map->erasePickableObject(pickableObjectID);
	}


	if (objectPickedUp != nullptr) {
		objectPickedUp->update(deltaTime);
		objectPickedUp->setPosicio(glm::vec2(posPlayer.x + 8, posPlayer.y - objectPickedUp->getMeasures().y + 10));
		//cout << "Object position: " << objectPickedUp->getPosicio().x << " " << objectPickedUp->getPosicio().y << endl;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	//cout << "Player position: " << posPlayer.x << " " << posPlayer.y << endl;	
}

void Player::render()
{
	if (objectPickedUp != nullptr) objectPickedUp->render();
	if (isHurt) sprite->setAlpha(0.3);
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

	//PICKING_OBJECT_LEFT
	sprite->setAnimationSpeed(PICKING_LEFT, 8);
	sprite->addKeyframe(PICKING_LEFT, glm::vec2(10 * SPRITE_WIDTH, 0.f));
	sprite->addKeyframe(PICKING_LEFT, glm::vec2(10 * SPRITE_WIDTH, SPRITE_HEIGHT));
	sprite->addKeyframe(PICKING_LEFT, glm::vec2(10 * SPRITE_WIDTH, 2 * SPRITE_HEIGHT));
	sprite->addKeyframe(PICKING_LEFT, glm::vec2(10 * SPRITE_WIDTH, 3 * SPRITE_HEIGHT));

	//PICKING_OBJECT_RIGHT
	sprite->setAnimationSpeed(PICKING_RIGHT, 8);
	sprite->addKeyframe(PICKING_RIGHT, glm::vec2(9 * SPRITE_WIDTH, 0.f));
	sprite->addKeyframe(PICKING_RIGHT, glm::vec2(9 * SPRITE_WIDTH, SPRITE_HEIGHT));
	sprite->addKeyframe(PICKING_RIGHT, glm::vec2(9 * SPRITE_WIDTH, 2 * SPRITE_HEIGHT));
	sprite->addKeyframe(PICKING_RIGHT, glm::vec2(9 * SPRITE_WIDTH, 3 * SPRITE_HEIGHT));

	//STANDING_LEFT_OBJECT
	sprite->setAnimationSpeed(STAND_LEFT_OBJECT, 8);
	sprite->addKeyframe(STAND_LEFT_OBJECT, glm::vec2(10 * SPRITE_WIDTH, 4 * SPRITE_HEIGHT));
	sprite->addKeyframe(STAND_LEFT_OBJECT, glm::vec2(10 * SPRITE_WIDTH, 5 * SPRITE_HEIGHT));

	//STANDING_RIGHT_OBJECT
	sprite->setAnimationSpeed(STAND_RIGHT_OBJECT, 8);
	sprite->addKeyframe(STAND_RIGHT_OBJECT, glm::vec2(9 * SPRITE_WIDTH, 4 * SPRITE_HEIGHT));
	sprite->addKeyframe(STAND_RIGHT_OBJECT, glm::vec2(9 * SPRITE_WIDTH, 5 * SPRITE_HEIGHT));


	//MOVING_LEFT_OBJECT
	sprite->setAnimationSpeed(MOVING_LEFT_OBJECT, 8);
	sprite->addKeyframe(MOVING_LEFT_OBJECT, glm::vec2(14 * SPRITE_WIDTH, 0.f));
	sprite->addKeyframe(MOVING_LEFT_OBJECT, glm::vec2(14 * SPRITE_WIDTH, SPRITE_HEIGHT));
	sprite->addKeyframe(MOVING_LEFT_OBJECT, glm::vec2(14 * SPRITE_WIDTH, 2 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVING_LEFT_OBJECT, glm::vec2(14 * SPRITE_WIDTH, 3 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVING_LEFT_OBJECT, glm::vec2(14 * SPRITE_WIDTH, 4 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVING_LEFT_OBJECT, glm::vec2(14 * SPRITE_WIDTH, 5 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVING_LEFT_OBJECT, glm::vec2(14 * SPRITE_WIDTH, 6 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVING_LEFT_OBJECT, glm::vec2(14 * SPRITE_WIDTH, 7 * SPRITE_HEIGHT));

	//MOVING_RIGHT_OBJECT
	sprite->setAnimationSpeed(MOVING_RIGHT_OBJECT, 8);
	sprite->addKeyframe(MOVING_RIGHT_OBJECT, glm::vec2(13 * SPRITE_WIDTH, 0.f));
	sprite->addKeyframe(MOVING_RIGHT_OBJECT, glm::vec2(13 * SPRITE_WIDTH, SPRITE_HEIGHT));
	sprite->addKeyframe(MOVING_RIGHT_OBJECT, glm::vec2(13 * SPRITE_WIDTH, 2 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVING_RIGHT_OBJECT, glm::vec2(13 * SPRITE_WIDTH, 3 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVING_RIGHT_OBJECT, glm::vec2(13 * SPRITE_WIDTH, 4 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVING_RIGHT_OBJECT, glm::vec2(13 * SPRITE_WIDTH, 5 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVING_RIGHT_OBJECT, glm::vec2(13 * SPRITE_WIDTH, 6 * SPRITE_HEIGHT));
	sprite->addKeyframe(MOVING_RIGHT_OBJECT, glm::vec2(13 * SPRITE_WIDTH, 7 * SPRITE_HEIGHT));

	sprite->changeAnimation(true, 2);
}

int Player::getPlayerLifes()
{
	return playerLifes;
}


void Player::updatePlayerMovement(int deltaTime) {

	bool KeysPressed = Game::instance().getKey(GLFW_KEY_W) || Game::instance().getKey(GLFW_KEY_A) || Game::instance().getKey(GLFW_KEY_S) || Game::instance().getKey(GLFW_KEY_D);
	bool ladderCollision = map->ladderCollision(posPlayer, glm::vec2(32, 32));
	if (!ladderCollision) {
		playerState.Climbing = false;
		playerState.isOnLadderTop = false;
	}
	if (Game::instance().getKey(GLFW_KEY_W) && ladderCollision || playerState.Jumping) playerKey_W(deltaTime);
	else if (!ladderCollision) playerFalling(deltaTime);

	if (Game::instance().getKey(GLFW_KEY_S)) { playerKey_S(deltaTime); loopTimesInactive = 0; }

	if (Game::instance().getKey(GLFW_KEY_A)) { playerKey_A(deltaTime); loopTimesInactive = 0; }

	if (!Game::instance().getKey(GLFW_KEY_A) and !Game::instance().getKey(GLFW_KEY_D)) { playerVelocity.x = 0; }

	if (Game::instance().getKey(GLFW_KEY_D)) { playerKey_D(deltaTime); loopTimesInactive = 0; }



	if (Game::instance().getKey(GLFW_KEY_ENTER)) { 
		if (objectPickedUp == nullptr and playerState.canPickObject) playerPickObject(deltaTime); 
		else if (objectPickedUp != nullptr and playerState.canDropObject) playerDropObject(deltaTime);
		loopTimesInactive = 0; 
	}
	else {
		if (objectPickedUp == nullptr) { playerState.canPickObject = true;  playerState.canDropObject = false; }
		if (objectPickedUp != nullptr) { playerState.canDropObject = true;  playerState.canPickObject = false; }
	}

	if (!KeysPressed) playerNOKeys(deltaTime);
}

void Player::playerKey_A(int deltaTime) {
	playerAcceleration.x = 0.01f;
	playerVelocity.x = min(2.f, playerVelocity.x + playerAcceleration.x * deltaTime);
	//cout << "A: " << playerVelocity.x << endl;
	glm::vec2 initialPosPlayer = posPlayer;
	posPlayer.x -= playerVelocity.x;

	if (isHurt != true  and map->lateralCollisionWithEnemy(posPlayer, glm::vec2(32, 32)) != -1) {
		isHurt = true;
		hurtTime = HURT_TIME;
		hurtFrameTime = HURT_FRAME_TIME;
	}

	if (!playerState.Jumping) {
		if (sprite->animation() != MOVE_LEFT and objectPickedUp == nullptr) sprite->changeAnimation(true, MOVE_LEFT);
		else if (sprite->animation() != MOVING_LEFT_OBJECT and objectPickedUp != nullptr)  sprite->changeAnimation(true, MOVING_LEFT_OBJECT);
	}
	else {
		if (sprite->animation() != JUMP_LEFT) sprite->changeAnimation(true, JUMP_LEFT);
	}

	// Poisicio final
	glm::vec2 targetPosPlayer = posPlayer;

	// Mirar colisions en el path del jugador
	for (float t = 0.0f; t <= 1.0f; t += 1.0f / playerVelocity.x) {
		glm::vec2 interpolatedPos = initialPosPlayer + t * (targetPosPlayer - initialPosPlayer);


		if (map->collisionMoveLeft(interpolatedPos, glm::ivec2(32, 32))) {
			posPlayer.x = initialPosPlayer.x;
			sprite->changeAnimation(true, STAND_LEFT);
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

	if (isHurt != true and map->lateralCollisionWithEnemy(posPlayer, glm::vec2(32, 32)) != -1) {
		isHurt = true;
		hurtTime = HURT_TIME;
		hurtFrameTime = HURT_FRAME_TIME;
	}

	if (!playerState.Jumping) {
		if (sprite->animation() != MOVE_RIGHT and objectPickedUp == nullptr) sprite->changeAnimation(true, MOVE_RIGHT);
		else if (sprite->animation() != MOVING_RIGHT_OBJECT and objectPickedUp != nullptr)  sprite->changeAnimation(true, MOVING_RIGHT_OBJECT);

	}
	else {
		if (sprite->animation() != JUMP_RIGHT) sprite->changeAnimation(true, JUMP_RIGHT);
	}
	// Poisicio final
	glm::vec2 targetPosPlayer = posPlayer;

	// Mirar colisions en el path del jugador
	for (float t = 0.0f; t <= 1.0f; t += 1.0f / playerVelocity.x) {
		glm::vec2 interpolatedPos = initialPosPlayer + t * (targetPosPlayer - initialPosPlayer);

		if (map->collisionMoveRight(interpolatedPos, glm::ivec2(32, 32))) {
			posPlayer.x = initialPosPlayer.x;
			sprite->changeAnimation(true, STAND_RIGHT);
			break;
		}
	}
}

void Player::playerNOKeys(int deltaTime) {
	if (isHurt != true and map->lateralCollisionWithEnemy(posPlayer, glm::vec2(32, 32)) != -1) {
		isHurt = true;
		hurtTime = HURT_TIME;
		hurtFrameTime = HURT_FRAME_TIME;
	}
	switch (sprite->animation()) {

		case STAND_LEFT:
			if (loopTimesInactive >= 500) { sprite->changeAnimation(true, HELLO_LEFT); loopTimesInactive = 0; }
			else ++loopTimesInactive;
			//cout << "Loop: " << loopTimesInactive << endl;
			break;
		
		case STAND_RIGHT:
			if (loopTimesInactive >= 500) { sprite->changeAnimation(true, HELLO_RIGHT); loopTimesInactive = 0; }
			else ++loopTimesInactive;
			break;

		case MOVE_LEFT:
			sprite->changeAnimation(true, STOPPING_LEFT);
			break;

		case MOVE_RIGHT:
			sprite->changeAnimation(true, STOPPING_RIGHT);
			break;

		case STOPPING_LEFT:
			if (playerVelocity.x == 0) sprite->changeAnimation(true, STAND_LEFT);
			else playerVelocity.x = max(0.f, playerVelocity.x - playerAcceleration.x * deltaTime);
			//cout << "Vel: " << playerVelocity.x << endl;

			if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
			{
				posPlayer.x += playerVelocity.x;
				sprite->changeAnimation(true, STAND_LEFT);
				playerVelocity.x = 0;
			}

			else posPlayer.x -= playerVelocity.x;
			break;

		case STOPPING_RIGHT:
			if (playerVelocity.x == 0) sprite->changeAnimation(true, STAND_RIGHT);
			else playerVelocity.x = max(0.f, playerVelocity.x - playerAcceleration.x * deltaTime);
			//cout << "Vel: " << playerVelocity.x << endl;

			if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
			{
				posPlayer.x -= playerVelocity.x;
				sprite->changeAnimation(true, STAND_RIGHT);
				playerVelocity.x = 0;
			}

			else posPlayer.x += playerVelocity.x;
			break;

		case CROUCH_LEFT:
			sprite->changeAnimation(true, STAND_LEFT);
			break;

		case CROUCH_RIGHT:
			sprite->changeAnimation(true, STAND_RIGHT);
			break;

		case PICKING_LEFT:
			sprite->changeAnimation(true, STAND_LEFT_OBJECT);
			break;
		
		case MOVING_LEFT_OBJECT:
			sprite->changeAnimation(true, STAND_LEFT_OBJECT);
			break;

		case PICKING_RIGHT:
			sprite->changeAnimation(true, STAND_RIGHT_OBJECT);
			break;

		case MOVING_RIGHT_OBJECT:
			sprite->changeAnimation(true, STAND_RIGHT_OBJECT);
			break;


		default:
			break;
	}
}

void Player::playerFalling(int deltaTime)
{
	posPlayer.y += FALL_STEP;
	playerVelocity.y = FALL_STEP;

	/*if (map->lateralCollisionWithEnemy(posPlayer, glm::vec2(32, 32)) != -1) {
		isHurt = true;
		hurtTime = HURT_TIME;
	}*/
	if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
	{
		if (sprite->animation() == FALL_LEFT || sprite->animation() == JUMP_LEFT) sprite->changeAnimation(true, STAND_LEFT);
		else if (sprite->animation() == FALL_RIGHT || sprite->animation() == JUMP_RIGHT) sprite->changeAnimation(true, STAND_RIGHT);

		playerVelocity.y = 0;
		if (Game::instance().getKey(GLFW_KEY_W))
		{
			playerState.Jumping = true;
			jumpAngle = 0;
			startY = int(posPlayer.y);
		}
	}
	if (map->holeCollision(posPlayer, glm::vec2(32, 32))) {
		playerLifes = 0;
	}
}

void Player::playerKey_W(int deltaTime) {
	// fet per l'escala, adaptar metodes per quan tinguem objectes que no es poden traspassar

	if (isHurt != true and map->lateralCollisionWithEnemy(posPlayer, glm::vec2(32, 32)) != -1) {
		isHurt = true;
		hurtTime = HURT_TIME;
		hurtFrameTime = HURT_FRAME_TIME;
	}


	if (map->ladderCollision(posPlayer, glm::vec2(32, 32))) {
		playerState.Climbing = true;
		playerState.Jumping = false;
		if (!playerState.isOnLadderTop) {
			playerVelocity.y = 0.1 * deltaTime;
			posPlayer.y -= playerVelocity.y;
			if (sprite->animation() != CLIMB) sprite->changeAnimation(true, CLIMB);
			if (map->isOnLadderTop(posPlayer, glm::vec2(32, 32))) {
				playerState.isOnLadderTop = true;
				playerVelocity.y = 0;
				//cout << "on top of the ladder" << endl;
			}
		}
	}
	else {
		playerState.Climbing = false;
		playerState.isOnLadderTop = false;
		switch (sprite->animation()) {
		case MOVE_LEFT:
			sprite->changeAnimation(true, JUMP_LEFT);
			break;

		case MOVE_RIGHT:
			sprite->changeAnimation(true, JUMP_RIGHT);
			break;

		case STAND_LEFT:
			sprite->changeAnimation(true, JUMP_LEFT);
			break;

		case STAND_RIGHT:
			sprite->changeAnimation(true, JUMP_RIGHT);
			break;

		case CROUCH_LEFT:
			sprite->changeAnimation(true, JUMP_LEFT);
			break;

		case CROUCH_RIGHT:
			sprite->changeAnimation(true, JUMP_RIGHT);
			break;
		
		case CLIMB:
			sprite->changeAnimation(true, STAND_RIGHT);
			break;

		case HELLO_LEFT:
			sprite->changeAnimation(true, JUMP_LEFT);
			break;
		
		case HELLO_RIGHT:
			sprite->changeAnimation(true, JUMP_RIGHT);
			break;
		}
		playerVelocity.y = JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f);
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			playerState.Jumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = startY - playerVelocity.y;
			if (jumpAngle > 90)
				playerState.Jumping = (!map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y));
			if (jumpAngle <= 90) {	// player going up
				playerState.Jumping = !map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
			}
		}
	}
}

void Player::playerKey_S(int deltaTime)
{
	if (map->ladderCollision(posPlayer, glm::vec2(32,32))) {
		playerState.Climbing = true;
	}
	if (playerState.isOnLadderTop) {
		playerState.Climbing = true;
		playerState.isOnLadderTop = false;
		playerState.Jumping = false;
	}
	if (playerState.Climbing) {
		playerVelocity.y = 0.1f * deltaTime;
		posPlayer.y += playerVelocity.y;

		if (sprite->animation() != CLIMB) {
			sprite->changeAnimation(true, CLIMB);
		}
		if (map->isOnLadderBottom(posPlayer, glm::vec2(32, 32))) {
			playerState.Climbing = false;
			playerVelocity.y = 0;
			//cout << "on BOTTOM of the ladder" << endl;
		}
	}

	else if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT || sprite->animation() == HELLO_LEFT || sprite->animation() == STOPPING_LEFT) sprite->changeAnimation(true, CROUCH_LEFT);
	else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT || sprite->animation() == HELLO_RIGHT || sprite->animation() == STOPPING_RIGHT) sprite->changeAnimation(true, CROUCH_RIGHT);

	else if (sprite->animation() == JUMP_LEFT and objectPickedUp == nullptr) sprite->changeAnimation(true, FALL_LEFT);
	else if (sprite->animation() == JUMP_RIGHT and objectPickedUp == nullptr) sprite->changeAnimation(true, FALL_RIGHT);

	if (sprite->animation() == FALL_LEFT or sprite->animation() == FALL_RIGHT) {
		int collidedEnemyId = map->verticalCollisionWithEnemy(posPlayer, glm::vec2(32, 32));
		if (collidedEnemyId != -1) {
			if (verticalCollisionTimeout <= 0) {
				map->eraseEnemy(collidedEnemyId);
				posPlayer.y -= playerVelocity.y;
				sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
				sprite->changeAnimation(true, STAND_RIGHT);
				verticalCollisionTimeout = VERTICAL_COL_TIMEOUT;
				// sumar punts al Player
			}
			else verticalCollisionTimeout -= deltaTime;
		}
		else {
			if (isHurt == false and map->lateralCollisionWithEnemy(posPlayer, glm::vec2(32, 32)) != -1) {
				isHurt = true;
				hurtTime = HURT_TIME;
				hurtFrameTime = HURT_FRAME_TIME;
			}
		}

		int collideWithChest = map->collisionWithChest(posPlayer, glm::vec2(32, 32));
	}
	if (sprite->animation() != FALL_LEFT && sprite->animation() != FALL_RIGHT) {
		
	}
}

void Player::playerPickObject(int deltaTime) {
	playerState.PickingObject = !playerState.PickingObject;
	int objectIndex = map -> pickingObject(posPlayer, glm::vec2(32,32));
	
	cout << "Object index: " << objectIndex << endl;
	if (objectIndex != -1) {
		objectPickedUp = map->getDynamicObject(objectIndex);
		objectPickedUp->pickObject();
		if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT || sprite->animation() == HELLO_LEFT || sprite->animation() == STOPPING_LEFT) sprite->changeAnimation(true, PICKING_LEFT);
		else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT || sprite->animation() == HELLO_RIGHT || sprite->animation() == STOPPING_RIGHT) sprite->changeAnimation(true, PICKING_RIGHT);
	}
}

void Player::playerDropObject(int deltaTime) {
	playerState.PickingObject = !playerState.PickingObject;

	DynamicObject* object = objectPickedUp;
	objectPickedUp = nullptr;

	if (sprite->animation() == JUMP_LEFT or sprite->animation() == MOVING_LEFT_OBJECT) object->dropObject(-playerVelocity.x);
	else if (sprite->animation() == JUMP_RIGHT or sprite->animation() == MOVING_RIGHT_OBJECT) object->dropObject(playerVelocity.x);
	else object->dropObject(0);

	if (sprite->animation() == PICKING_LEFT) sprite->changeAnimation(true, STAND_LEFT);
	else if (sprite->animation() == PICKING_RIGHT) sprite->changeAnimation(true, STAND_RIGHT);
}



