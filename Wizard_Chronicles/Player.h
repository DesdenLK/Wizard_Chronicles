#ifndef _Player_INCLUDE
#define _Player_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the Player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void updatePlayerMovement(int deltaTime);
	void PlayerKey_A(int deltaTime);
	void PlayerKey_D(int deltaTime);
	void PlayerKey_W(int deltaTime);
	void PlayerKey_S(int deltaTime);
	void PlayerNOKeys(int deltaTime);

	void PlayerFalling(int deltaTime);
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setAnimations();



	glm::vec2 getPosition();
	glm::vec2 getVelocity();
	
private:
	bool Jumping, Climbing, isOnLadderTop, isHurt;
	int loopTimesInactive, hurtTime, verticalCollisionTimeout;
	glm::ivec2 tileMapDispl;
	glm::vec2 posPlayer, PlayerVelocity, PlayerAcceleration;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _Player_INCLUDE


