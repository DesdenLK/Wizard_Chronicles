#ifndef _INSTRUCTIONS_INCLUDE
#define _INSTRUCTIONS_INCLUDE
#include "Scene.h"
#include "Game.h"

class Instructions : public Scene
{
	public:
		Instructions();
		~Instructions();

		void init() override;
		void update(int deltaTime) override;
		void render() override;

	private:
		Sprite* background;
		Texture spritesheet;

		void setAnimations();
};

#endif // _INSTRUCTIONS__INCLUDE

