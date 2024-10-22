#ifndef _Enemy_INCLUDE
#define _Enemy_INCLUDE

#include "Sprite.h"
#include "TileMap.h"


// Abstract class Enemy from which each enemy can inherit with their specifics


class Enemy
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string pathToSpritesheet, const glm::ivec2& spriteDimensions, const glm::vec2& spriteWidthHeight);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void EnemyFalling(int deltaTime);
	void setPosition(const glm::vec2& pos);

	virtual void updateEnemyMovement(int deltaTime) = 0;
	virtual void setAnimations() = 0;

	glm::vec2 getPosition();
	glm::vec2 getVelocity();

protected:
	bool Jumping;
	int loopTimesInactive;
	glm::ivec2 tileMapDispl;
	glm::vec2 posEnemy, EnemyVelocity, EnemyAcceleration;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};


#endif // _Enemy_INCLUDE