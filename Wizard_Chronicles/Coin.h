#ifndef _COIN_INCLUDE
#define _COIN_INCLUDE
#include "PickableObject.h"
class Coin : public PickableObject
{
	public:
		void init(int id, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, ShaderProgram& shaderProgram, TileMap* map) override;
		void setAnimations();

		string getType() override { return "Coin"; }
};

#endif // _COIN_INCLUDE
