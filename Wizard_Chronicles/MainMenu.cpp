#include "MainMenu.h"

#include "StartScene.h"
#include "Game.h"

enum StartSceneAnims
{
	SELECT_ONE, SELECT_TWO, SELECT_THREE, PAGE1, PAGE2, NUM_ANIMS
};

MainMenu::MainMenu()
{
	background = NULL;
	selection = 0;
}

MainMenu::~MainMenu()
{
	if (background != NULL)
		delete background;
}

void MainMenu::init()
{
	Scene::init();

	selection = SELECT_ONE;

	keyDown, keyUp, keyLeft, keyRight = false;

	spritesheet.loadFromFile("images/MainMenu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(0.25, 0.5), &spritesheet, &texProgram);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	setAnimations();
}

void MainMenu::update(int deltaTime)
{
	if (Game::instance().getKey(GLFW_KEY_RIGHT) and background->animation() == PAGE2 and not keyRight) {
		background->changeAnimation(true, SELECT_ONE);
		selection = SELECT_ONE;
		keyRight = true;
	}
	else if (not Game::instance().getKey(GLFW_KEY_RIGHT) and keyRight) keyRight = false;

	if (Game::instance().getKey(GLFW_KEY_DOWN) and not keyDown) {
		selection = (selection + 1) % 3;
		background->changeAnimation(true, selection);
		keyDown = true;
	}
	else if (not Game::instance().getKey(GLFW_KEY_DOWN) and keyDown) keyDown = false;

	if (Game::instance().getKey(GLFW_KEY_UP) and not keyUp) {
		selection = selection - 1;
		if (selection < 0) selection = 2;
		background->changeAnimation(true, selection);
		keyUp = true;
	}
	else if (not Game::instance().getKey(GLFW_KEY_UP) and keyUp) keyUp = false;

	if (Game::instance().getKey(GLFW_KEY_ENTER) and background->animation() == SELECT_THREE) {
		background->changeAnimation(true, PAGE1);
	}

	if (Game::instance().getKey(GLFW_KEY_RIGHT) and not keyRight and background->animation() == PAGE1) {
		background->changeAnimation(true, PAGE2);
		keyRight = true;
	}
	else if (not Game::instance().getKey(GLFW_KEY_RIGHT) and keyRight) keyRight = false;

	if (Game::instance().getKey(GLFW_KEY_LEFT) and not keyLeft and background->animation() == PAGE2) {
		background->changeAnimation(true, PAGE1);
		keyRight = true;
	}
	else if (not Game::instance().getKey(GLFW_KEY_LEFT) and keyLeft) keyLeft = false;


	currentTime += deltaTime;
	background->update(deltaTime);
}

void MainMenu::render()
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	background->render();
}

bool MainMenu::inInstructions()
{
	if (background->animation() >= SELECT_THREE) return true;

	return false;
}

int MainMenu::getLevel()
{
	if (background->animation() == SELECT_ONE) return 0;
	else if (background->animation() == SELECT_TWO) return 1;
	else return -1;
}

void MainMenu::setAnimations()
{
	background->setNumberAnimations(NUM_ANIMS);

	background->setAnimationSpeed(SELECT_ONE, 1);
	background->addKeyframe(SELECT_ONE, glm::vec2(0.f, 0.f));

	background->setAnimationSpeed(SELECT_TWO, 1);
	background->addKeyframe(SELECT_TWO, glm::vec2(0.25, 0.f));

	background->setAnimationSpeed(SELECT_THREE, 1);
	background->addKeyframe(SELECT_THREE, glm::vec2(0.5, 0.f));

	background->setAnimationSpeed(PAGE1, 1);
	background->addKeyframe(PAGE1, glm::vec2(0, 0.5));

	background->setAnimationSpeed(PAGE2, 1);
	background->addKeyframe(PAGE2, glm::vec2(0.25, 0.5));





	background->changeAnimation(true, SELECT_ONE);
}
