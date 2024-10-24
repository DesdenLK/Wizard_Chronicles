#ifndef _PICKABLE_OBJECT_INCLUDE
#define _PICKABLE_OBJECT_INCLUDE
#include "Object.h"
#include "Sprite.h"
#include "Texture.h"



class TileMap;

class PickableObject : public Object
{
protected:
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;

public:
    virtual void init(int id, std::string pathToFile, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, ShaderProgram& shaderProgram, TileMap* map);

    void render();
    void update(int deltaTime);


    //virtual void destroyObject();

    //virtual void setAnimations();

    bool isPickable() override;
};

#endif // _PICKABLE_OBJECT_INCLUDE

