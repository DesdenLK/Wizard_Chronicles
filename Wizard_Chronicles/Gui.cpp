#include "Gui.h"

enum HeartsAnimations
{
	LIFE, TWOLIFES, THREELIFES, NOLIFES, NUM_HEARTS_ANIMATIONS
};

void Gui::init(glm::vec2 position, glm::vec2 quadSize, float spriteWidth, float spriteHeight, ShaderProgram& shaderProgram)
{

	spritesheet.loadFromFile("images/Gui.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(quadSize, glm::vec2(spriteWidth, spriteHeight), &spritesheet, &shaderProgram);

	sprite->setPosition(position);

	heartsSpriteSheet.loadFromFile("images/Hearts.png", TEXTURE_PIXEL_FORMAT_RGBA);
	heartsSprite = Sprite::createSprite(glm::vec2(128,32), glm::vec2(1, 0.25), &heartsSpriteSheet, &shaderProgram);

	heartsSprite->setPosition(position + glm::vec2(20, 435));

	setAnimations();

}

void Gui::update(int deltaTime, int numberOfLifes)
{
	sprite->update(deltaTime);
	heartsSprite->update(deltaTime);

	
	if (numberOfLifes == 3) heartsSprite->changeAnimation(true, THREELIFES);
	else if (numberOfLifes == 2) heartsSprite->changeAnimation(true, TWOLIFES);
	else if (numberOfLifes == 1) heartsSprite->changeAnimation(true, LIFE);
	else if (numberOfLifes == 0) heartsSprite->changeAnimation(true, NOLIFES);
	
}

void Gui::render()
{
	sprite->render();
	heartsSprite->render();
}

void Gui::setAnimations()
{
	heartsSprite->setNumberAnimations(NUM_HEARTS_ANIMATIONS);

	heartsSprite->setAnimationSpeed(THREELIFES, 8);
	heartsSprite->addKeyframe(THREELIFES, glm::vec2(0, 0.75));

	heartsSprite->setAnimationSpeed(TWOLIFES, 8);
	heartsSprite->addKeyframe(TWOLIFES, glm::vec2(0, 0.5));

	heartsSprite->setAnimationSpeed(LIFE, 8);
	heartsSprite->addKeyframe(LIFE, glm::vec2(0, 0.25));

	heartsSprite->setAnimationSpeed(NOLIFES, 8);
	heartsSprite->addKeyframe(NOLIFES, glm::vec2(0, 0));

	heartsSprite->changeAnimation(true, LIFE);
}
