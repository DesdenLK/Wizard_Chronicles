#ifndef _CAKE_INCLUDE
#define _CAKE_INCLUDE
#include "PickableObject.h"
class Cake : public PickableObject
{
	public:
		void init(int id, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, ShaderProgram& shaderProgram, TileMap* map) override;
		void setAnimations();

		string getType() override { return "Cake"; }
};

#endif // _CAKE_INCLUDE

