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

	NumbersSpriteSheet.loadFromFile("images/Numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	Tries = vector<Sprite*>(2);
	for (int i = 0; i < 2; i++) {
		Tries[i] = Sprite::createSprite(glm::vec2(20, 20), glm::vec2(0.1, 1), &NumbersSpriteSheet, &shaderProgram);
		Tries[i]->setPosition(position + glm::vec2(170 + i * 20, 440));
	}


	Score = vector<Sprite*>(6);
	for (int i = 0; i < 6; i++) {
		Score[i] = Sprite::createSprite(glm::vec2(15, 15), glm::vec2(0.1, 1), &NumbersSpriteSheet, &shaderProgram);
		Score[i]->setPosition(position + glm::vec2(350 + i * 15, 440));
	}

	Time = vector<Sprite*>(4);
	for (int i = 0; i < 4; i++) {
		Time[i] = Sprite::createSprite(glm::vec2(15, 15), glm::vec2(0.1, 1), &NumbersSpriteSheet, &shaderProgram);
		Time[i]->setPosition(position + glm::vec2(540 + i * 15, 440));
	}
	setAnimations();

}

void Gui::update(int deltaTime, int numberOfLifes, int tries, int score, int time)
{
	sprite->update(deltaTime);
	heartsSprite->update(deltaTime);

	
	if (numberOfLifes == 3) heartsSprite->changeAnimation(true, THREELIFES);
	else if (numberOfLifes == 2) heartsSprite->changeAnimation(true, TWOLIFES);
	else if (numberOfLifes == 1) heartsSprite->changeAnimation(true, LIFE);
	else if (numberOfLifes == 0) heartsSprite->changeAnimation(true, NOLIFES);

	updateTries(deltaTime, tries);
	updateScore(deltaTime, score);
	updateTime(deltaTime, time);
	
}

void Gui::render()
{
	sprite->render();
	heartsSprite->render();

	for (int i = 0; i < 2; i++) {
		Tries[i]->render();
	}

	for (int i = 0; i < 6; i++) {
		Score[i]->render();
	}

	for (int i = 0; i < 4; i++) {
		Time[i]->render();
	}
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

	for (int i = 0; i < 2; i++) {
		Tries[i]->setNumberAnimations(10);
		for (int j = 0; j < 10; j++) {
			Tries[i]->setAnimationSpeed(j, 8);
			Tries[i]->addKeyframe(j, glm::vec2(j * 0.1, 0));
		}
		Tries[i]->changeAnimation(true, 0);
	}

	for (int i = 0; i < 6; i++) {
		Score[i]->setNumberAnimations(10);
		for (int j = 0; j < 10; j++) {
			Score[i]->setAnimationSpeed(j, 8);
			Score[i]->addKeyframe(j, glm::vec2(j * 0.1, 0));
		}
		Score[i]->changeAnimation(true, 0);
	}

	for (int i = 0; i < 4; i++) {
		Time[i]->setNumberAnimations(10);
		for (int j = 0; j < 10; j++) {
			Time[i]->setAnimationSpeed(j, 8);
			Time[i]->addKeyframe(j, glm::vec2(j * 0.1, 0));
		}
		Time[i]->changeAnimation(true, 0);
	}
}

void Gui::updateTries(int deltaTime, int tries)
{
	for (int i = 1; i >= 0; i--) {
		if (tries % 10 != Tries[i]->animation()) Tries[i]->changeAnimation(true, tries % 10);
		Tries[i]->update(deltaTime);
		tries /= 10;
	}
}

void Gui::updateScore(int deltaTime, int score)
{
	for (int i = 5; i >= 0; i--) {
		if (score % 10 != Score[i]->animation()) Score[i]->changeAnimation(true, score % 10);
		Score[i]->update(deltaTime);
		score /= 10;
	}
}

void Gui::updateTime(int deltaTime, int time)
{
	for (int i = 3; i >= 0; i--) {
		if (time % 10 != Time[i]->animation()) Time[i]->changeAnimation(true, time % 10);
		Time[i]->update(deltaTime);
		time /= 10;
	}
}
