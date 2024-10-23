#pragma once
#include "DynamicObject.h"
class DynamicObjectBox : public DynamicObject
{
public:
	void init(string pathToFile, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, glm::vec2 offSet, ShaderProgram& shaderProgram, TileMap* map) override;
	void setAnimations();
	
};

