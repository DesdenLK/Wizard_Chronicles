#include "Coin.h"


#define COIN_SPRITE_SIZE_X 0.25
#define COIN_SPRITE_SIZE_Y 0.25

enum CakeAnims { COIN, NUM_ANIMS };

void Coin::init(int id, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, ShaderProgram& shaderProgram, TileMap* map)
{
	this->id = id;
	posicio = glm::vec2(x, y);
	measures = glm::vec2(w, h);
	speed = glm::vec2(0, 0);

	jumpAngle = 0;
	startY = posicio.y;
	objectState = { true, false };

	this->map = map;




	spritesheet.loadFromFile("images/DynamicObjects/Coin.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(quadSize, glm::vec2(spriteWidth, spriteHeight), &spritesheet, &shaderProgram);

	sprite->setPosition(posicio);

	setAnimations();
}

void Coin::setAnimations()
{
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(COIN, 2);
	sprite->addKeyframe(COIN, glm::vec2(0, 0));
	sprite->addKeyframe(COIN, glm::vec2(0.25, 0));
	sprite->addKeyframe(COIN, glm::vec2(0.5, 0));
	sprite->addKeyframe(COIN, glm::vec2(0.75, 0));




	sprite->changeAnimation(true, COIN);
}


