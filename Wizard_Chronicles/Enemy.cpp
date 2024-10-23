#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Enemy.h"
#include "Game.h"
#include "TileMap.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 30
#define FALL_STEP 4
#define INI_VELOCITY_X 0.5f
#define INI_VELOCITY_Y 0


void Enemy::init(int id, const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string pathToSpritesheet, const glm::ivec2& spriteDimensions, const glm::vec2& spriteWidthHeight, const glm::vec2& hitboxWH, int eraseAnimTime)
{
	this->id = id;
	eraseAnimationTime = eraseAnimTime;
	boundingBoxWidthHeight = hitboxWH;
	Jumping = false;
	loopTimesInactive = 0;
	EnemyVelocity = glm::vec2(INI_VELOCITY_X, INI_VELOCITY_Y);
	EnemyAcceleration = glm::vec2(0, 0);

	bool loaded = spritesheet.loadFromFile(pathToSpritesheet, TEXTURE_PIXEL_FORMAT_RGBA);
	if (!loaded) cout << "texture could not load" << endl;
	else cout << "texture loaded" << endl;
	sprite = Sprite::createSprite(spriteDimensions, spriteWidthHeight, &spritesheet, &shaderProgram);

	setAnimations();

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));

}

void Enemy::update(int deltaTime)
{
	sprite->update(deltaTime);
	updateEnemyMovement(deltaTime);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::render()
{
	sprite->render();
}

void Enemy::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

int Enemy::getId() {
	return id;
}

glm::vec2 Enemy::getPosition() {
	return posEnemy;
}

void Enemy::setPosition(const glm::vec2& pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

glm::vec2 Enemy::getVelocity() {
	return EnemyVelocity;
}

glm::vec2 Enemy::getBoundingBoxWH() {
	return boundingBoxWidthHeight;
}

void Enemy::EnemyFalling(int deltaTime)
{
	posEnemy.y += FALL_STEP;
	EnemyVelocity.y = FALL_STEP;
	if (map->collisionMoveDown(posEnemy, glm::ivec2(32, 32), &posEnemy.y))
	{
		/*if (sprite->animation() == FALL_LEFT || sprite->animation() == JUMP_LEFT) sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == FALL_RIGHT || sprite->animation() == JUMP_RIGHT) sprite->changeAnimation(STAND_RIGHT);*/

		EnemyVelocity.y = 0;
		/*if (Game::instance().getKey(GLFW_KEY_W))
		{
			Jumping = true;
			jumpAngle = 0;
			startY = int(posEnemy.y);
		}*/
	}
}
