#ifndef _DYNAMIC_OBJECT_INCLUDE
#define _DYNAMIC_OBJECT_INCLUDE
#include "Object.h";
#include "Sprite.h"
#include "Texture.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 20
#define FALL_STEP 1


class TileMap;

class DynamicObject : public Object {
protected:
	struct ObjectState {
		bool Jumping, pickedUp, objectThrowed, destroyed, hitboxEnabled;
	}objectState;

	int jumpAngle;
	float startY;

	int loopTimesDestroy;

	glm::vec2 speed;
	glm::vec2 spriteOffset;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

public:
	virtual void init(int id, string pathToFile, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, glm::vec2 offSet, ShaderProgram& shaderProgram, TileMap *map);

	void render();
	void update(int deltaTime);

	virtual void destroyObject();

	virtual void objectFalling();
	virtual void objectJump();

	virtual void setPickableObject(string item);


	//virtual void setAnimations();



	glm::vec2 getSpeed() const;

	void pickObject();
	void dropObject(float XSpeed);
	//void throwObject();


	bool IsPickedUp() const;
	bool IsDestroyed() const;
	bool IsHitboxEnabled() const;

	void setSpeed(glm::vec2 speed);
};

#endif // !_DYNAMIC_OBJECT_INCLUDE
