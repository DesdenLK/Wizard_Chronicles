#ifndef _Enemy_INCLUDE
#define _Enemy_INCLUDE

#include "Sprite.h"

// Abstract class Enemy from which each enemy can inherit with their specifics

class TileMap;

class Enemy
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string pathToSpritesheet, const glm::ivec2& spriteDimensions, const glm::vec2& spriteWidthHeight, const glm::vec2& hitboxWH);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void EnemyFalling(int deltaTime);
	void setPosition(const glm::vec2& pos);

	virtual void updateEnemyMovement(int deltaTime) = 0;
	virtual void setAnimations() = 0;

	glm::vec2 getPosition();
	glm::vec2 getVelocity();
	glm::vec2 getBoundingBoxWH();

protected:
	bool Jumping;
	int loopTimesInactive;
	glm::ivec2 tileMapDispl;
	glm::vec2 posEnemy, EnemyVelocity, EnemyAcceleration;
	glm::vec2 boundingBoxWidthHeight;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};


#endif // _Enemy_INCLUDE