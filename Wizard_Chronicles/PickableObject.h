#ifndef _PICKABLE_OBJECT_INCLUDE
#define _PICKABLE_OBJECT_INCLUDE
#include "Object.h"
#include "Sprite.h"
#include "Texture.h"

#define JUMP_ANGLE_STEP_PICKOBJ 4
#define JUMP_HEIGHT_PICKOBJ 10
#define FALL_STEP_PICKOBJ 1



class TileMap;

class PickableObject : public Object
{
protected:
	struct ObjectState
	{
		bool Jumping;
		bool Falling;
	}objectState;

	int jumpAngle;
    int startY;

    glm::vec2 speed;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;


public:
    virtual void init(int id, string pathToFile, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, ShaderProgram& shaderProgram, TileMap* map) = 0;

    void render();
    void update(int deltaTime);

	virtual void objectFalling();
	virtual void objectJump();


    //virtual void destroyObject();

    //virtual void setAnimations();

    bool isPickable() override;
    virtual string getType() = 0;
};

#endif // _PICKABLE_OBJECT_INCLUDE

