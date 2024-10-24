#ifndef _DYNAMIC_OBJECT_INCLUDE
#define _DYNAMIC_OBJECT_INCLUDE
#include "Object.h";
#include "Sprite.h"
#include "Texture.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 10
#define FALL_STEP 1


class TileMap;

class DynamicObject : public Object {
protected:
	struct ObjectState {
		bool Jumping, pickedUp, objectThrowed;
	}objectState;

	int jumpAngle;
	float startY;

	glm::vec2 speed;
	glm::vec2 spriteOffset;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

public:
	virtual void init(string pathToFile, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, glm::vec2 offSet, ShaderProgram& shaderProgram, TileMap *map);

	void render();
	void update(int deltaTime);

	virtual void objectFalling();
	virtual void objectJump();

	//virtual void setAnimations();



	glm::vec2 getSpeed() const;

	void pickObject();
	void dropObject(float XSpeed);
	//void throwObject();


	bool IsPickedUp() const;
	void setSpeed(glm::vec2 speed);
};

#endif // !_DYNAMIC_OBJECT_INCLUDE
