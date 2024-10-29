#ifndef _GEM_PICKABLE_INCLUDE
#define _GEM_PICKABLE_INCLUDE

#include "PickableObject.h"

#define JUMP_ANGLE_STEP_PICKOBJ 4
#define JUMP_HEIGHT_PICKOBJ 10
#define FALL_STEP_PICKOBJ 1

class TileMap;

class GemPickable :
    public PickableObject
{
public:
    void render();
    void update(int deltaTime);

    virtual void objectFalling() override;
    virtual void objectJump() override;

    virtual void init(int id, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, ShaderProgram& shaderProgram, TileMap* map) override;
    virtual string getType() override;
    //virtual void destroyObject();
};

#endif