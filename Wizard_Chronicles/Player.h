#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void updatePlayerMovement(int deltaTime);
	void playerKey_A(int deltaTime);
	void playerKey_D(int deltaTime);
	void playerNOKeys(int deltaTime);
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setAnimations();



	glm::vec2 getPosition();
	glm::vec2 getVelocity();
	
private:
	bool Jumping, Falling;
	glm::ivec2 tileMapDispl;
	glm::vec2 posPlayer, playerVelocity, playerAcceleration;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE


