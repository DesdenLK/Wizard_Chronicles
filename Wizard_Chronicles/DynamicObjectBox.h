#ifndef _DYNAMIC_OBJECT_BOX_INCLUDE
#define _DYNAMIC_OBJECT_BOX_INCLUDE
#include "DynamicObject.h"  
class DynamicObjectBox : public DynamicObject  
{  
public:  
void init(int id, string pathToFile, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, glm::vec2 offSet, ShaderProgram& shaderProgram, TileMap* map) override;  
void setAnimations();  

void objectFalling() override;  
void objectJump() override;  

void destroyObject() override;  
};
#endif // _DYNAMIC_OBJECT_BOX_INCLUDE
