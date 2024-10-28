#include "StartScene.h"
#include "Game.h"

enum StartSceneAnims
{
	START, NUM_ANIMS
};

StartScene::StartScene()
{
	background = NULL;
}

StartScene::~StartScene()
{
	if (background != NULL)
		delete background;
}

void StartScene::init()
{
	Scene::init();
	spritesheet.loadFromFile("images/StartScene.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(0.25, 1), &spritesheet, &texProgram);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	setAnimations();
}

void StartScene::update(int deltaTime)
{
	currentTime += deltaTime;
	background->update(deltaTime);
}

void StartScene::render()
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

void StartScene::setAnimations()
{
	background->setNumberAnimations(NUM_ANIMS);
	background->setAnimationSpeed(START, 2);
	background->addKeyframe(0, glm::vec2(0.f, 0.f));
	background->addKeyframe(0, glm::vec2(0.25f, 0.f));
	background->addKeyframe(0, glm::vec2(0.5f, 0.f));



	background->changeAnimation(true, START);
}