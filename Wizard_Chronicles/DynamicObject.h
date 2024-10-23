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
		bool Jumping, pickedUp;
	}objectState;

	int jumpAngle;
	float startY;

	glm::vec2 speed;
	glm::vec2 spriteOffset;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

public:
	void init(string pathToFile, float x, float y, float w, float h, glm::vec2 quadSize, int spriteWidth, int spriteHeight, glm::vec2 offSet, ShaderProgram& shaderProgram, TileMap *map);
	//void update(int deltaTime);
	void render();
	void update(int deltaTime);

	void objectFalling();
	void objectJump();


	glm::vec2 getSpeed() const;

	void pickObject();
	void dropObject();
	//void throwObject();


	bool IsPickedUp() const;
	void setSpeed(glm::vec2 speed);
};

#endif // !_DYNAMIC_OBJECT_INCLUDE
