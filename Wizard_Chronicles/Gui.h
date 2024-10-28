#ifndef _GUI_INCLUDE
#define _GUI_INCLUDE

#include "Texture.h"
#include "Sprite.h"

class Gui
{
	public:
		void init(glm::vec2 position, glm::vec2 quadSize, float spriteWidth, float spriteHeight, ShaderProgram& shaderProgram);
		void update(int deltaTime, int numberOfLife, int tries, int score, int time);
		void render();

		void setAnimations();

	private:
		void updateTries(int deltaTime, int tries);
		void updateScore(int deltaTime, int score);
		void updateTime(int deltaTime, int time);

	private:
		Texture spritesheet;
		Sprite* sprite;

		Texture heartsSpriteSheet;
		Sprite* heartsSprite;

		Texture NumbersSpriteSheet;


		vector<Sprite*> Tries;
		vector<Sprite*> Score;
		vector<Sprite*> Time;
};

#endif // _GUI_INCLUDE

