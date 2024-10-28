#ifndef _MAIN_MENU_INCLUDE
#define _MAIN_MENU_INCLUDE
#include "Scene.h"
class MainMenu : public Scene
{
public:
	MainMenu();
	~MainMenu();

	void init() override;
	void update(int deltaTime) override;
	void render() override;

	bool inInstructions() override;
	int getLevel() override;

private:
	Sprite* background;
	Texture spritesheet;
	int selection;

	bool keyUp, keyDown, keyLeft, keyRight;

	void setAnimations();
};

#endif // _MAIN_MENU_INCLUDE

