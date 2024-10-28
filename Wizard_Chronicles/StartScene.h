#ifndef _START_SCENE_INCLUDE
#define _START_SCENE_INCLUDE
#include "Scene.h"
class StartScene : public Scene
{
	public:
		StartScene();
		~StartScene();

		void init() override;
		void update(int deltaTime) override;
		void render() override;

	private:
		Sprite* background;
		Texture spritesheet;

		void setAnimations();
};

#endif // _START_SCENE__INCLUDE

