#ifndef _Player_INCLUDE
#define _Player_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "DynamicObject.h"


// Player is basically a Sprite that represents the Player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

class Tilemap;

class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void updatePlayerMovement(int deltaTime);
	void playerKey_A(int deltaTime);
	void playerKey_D(int deltaTime);
	void playerKey_W(int deltaTime);
	void playerKey_S(int deltaTime);
	void playerNOKeys(int deltaTime);

	void playerPickObject(int deltaTime);
	void playerDropObject(int deltaTime);

	void playerFalling(int deltaTime);
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setAnimations();

	int getPlayerLifes();


	glm::vec2 getPosition();
	glm::vec2 getVelocity();
	
private:
	struct State {
		bool Jumping, Climbing, isOnLadderTop, PickingObject, canPickObject, canDropObject;
	} playerState;

	bool isHurt;
	int loopTimesInactive, hurtTime, hurtFrameTime, verticalCollisionTimeout;
	glm::ivec2 tileMapDispl;
	glm::vec2 posPlayer, playerVelocity, playerAcceleration;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

	DynamicObject* objectPickedUp;

	int playerLifes, playerScore;
	bool gPressed;
	bool godMode;

};


#endif // _Player_INCLUDE


