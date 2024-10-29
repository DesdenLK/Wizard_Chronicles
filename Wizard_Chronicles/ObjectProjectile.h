#ifndef _ObjectProjectile_INCLUDE
#define _ObjectProjectile_INCLUDE

#include "Object.h"
#include "ShaderProgram.h"
#include "Sprite.h"


class TileMap;

class ObjectProjectile :
    public Object
{
public:
    void init(int id, std::string pathToFile, glm::vec2 startPos, glm::vec2 quadSize, ShaderProgram& shaderProgram, TileMap* map, const glm::vec2& velocityDir);
    void setAnimations();

	glm::vec2 getPosition();
	glm::vec2 getBoundingBoxWH();

    void update(int deltaTime);
	void render();
    void destroyObject();
	bool isHitboxEnabled();

private:
	int loopTimesDestroy;
	bool hitboxEnabled;

	glm::vec2 velocityDir;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};

#endif