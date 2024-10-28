#ifndef _GUI_INCLUDE
#define _GUI_INCLUDE

#include "Texture.h"
#include "Sprite.h"

class Gui
{
	public:
		void init(glm::vec2 position, glm::vec2 quadSize, float spriteWidth, float spriteHeight, ShaderProgram& shaderProgram);
		void update(int deltaTime, int numberOfLife);
		void render();

		void setAnimations();

	private:
		Texture spritesheet;
		Sprite* sprite;

		Texture heartsSpriteSheet;
		Sprite* heartsSprite;
};

#endif // _GUI_INCLUDE

