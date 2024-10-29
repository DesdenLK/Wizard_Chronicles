#ifndef _DYNAMIC_OBJECT_CHEST_INCLUDE
#define _DYNAMIC_OBJECT_CHEST_INCLUDE
#include "DynamicObject.h"  

class DynamicObjectChest : public DynamicObject
{
	private:
		bool chestOpened;
		string pickableItem;
	public:
		void init(int id, string pathToFile, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, glm::vec2 offSet, ShaderProgram& shaderProgram, TileMap* map) override;
		void setAnimations();

		void objectFalling() override;
		void objectJump() override;

		void setPickableObject(string item) override;

		bool isChest() const override;

		void destroyObject() override;
		void openChest(bool playerDropping) override;
};
#endif // _DYNAMIC_OBJECT_CHEST_INCLUDE

