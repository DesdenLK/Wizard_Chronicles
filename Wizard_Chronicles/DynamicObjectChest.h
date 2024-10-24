#pragma once
#include "DynamicObject.h"  
class DynamicObjectChest : public DynamicObject
{
	public:
		void init(int id, string pathToFile, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, glm::vec2 offSet, ShaderProgram& shaderProgram, TileMap* map) override;
		void setAnimations();

		void objectFalling() override;
		void objectJump() override;

		void destroyObject() override;
		void openChest();
	};

