#include "Credit.h"

#include "StartScene.h"
#include "Game.h"

enum CreditSceneAnims
{
	START, NUM_ANIMS
};

Credit::Credit()
{
	background = NULL;
}

Credit::~Credit()
{
	if (background != NULL)
		delete background;
}

void Credit::init()
{
	Scene::init();
	spritesheet.loadFromFile("images/Credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1, 1), &spritesheet, &texProgram);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	setAnimations();
}

void Credit::update(int deltaTime)
{
	currentTime += deltaTime;
	background->update(deltaTime);
}

void Credit::render()
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

void Credit::setAnimations()
{
	background->setNumberAnimations(NUM_ANIMS);
	background->setAnimationSpeed(START, 2);
	background->addKeyframe(0, glm::vec2(0.f, 0.f));



	background->changeAnimation(true, START);
}
