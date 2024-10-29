#include "Cake.h"

#define CAKE_SPRITE_SIZE_X 0.25
#define CAKE_SPRITE_SIZE_Y 0.25

enum CakeAnims { CAKE, NUM_ANIMS };
void Cake::init(int id, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, ShaderProgram& shaderProgram, TileMap* map)
{
	this->id = id;
	posicio = glm::vec2(x, y);
	measures = glm::vec2(w, h);
	speed = glm::vec2(0, 0);

	jumpAngle = 0;
	startY = posicio.y;
	objectState = { true, false };

	this->map = map;





	spritesheet.loadFromFile("images/DynamicObjects/Cakes.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(quadSize, glm::vec2(spriteWidth, spriteHeight), &spritesheet, &shaderProgram);

	sprite->setPosition(posicio);

	setAnimations();
}

void Cake::setAnimations()
{
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(CAKE, 2);
	sprite->addKeyframe(CAKE, glm::vec2(0, 0));
	sprite->addKeyframe(CAKE, glm::vec2(CAKE_SPRITE_SIZE_X, 0));
	sprite->addKeyframe(CAKE, glm::vec2(2 * CAKE_SPRITE_SIZE_X, 0));
	sprite->addKeyframe(CAKE, glm::vec2(3 * CAKE_SPRITE_SIZE_X, 0));

	sprite->addKeyframe(CAKE, glm::vec2(0, 0));
	sprite->addKeyframe(CAKE, glm::vec2(CAKE_SPRITE_SIZE_X, CAKE_SPRITE_SIZE_Y));
	sprite->addKeyframe(CAKE, glm::vec2(2 * CAKE_SPRITE_SIZE_X, CAKE_SPRITE_SIZE_Y));
	sprite->addKeyframe(CAKE, glm::vec2(3 * CAKE_SPRITE_SIZE_X, CAKE_SPRITE_SIZE_Y));

	sprite->addKeyframe(CAKE, glm::vec2(0, 2 * CAKE_SPRITE_SIZE_Y));
	sprite->addKeyframe(CAKE, glm::vec2(CAKE_SPRITE_SIZE_X, 2 * CAKE_SPRITE_SIZE_Y));

	sprite->addKeyframe(CAKE, glm::vec2(0, 3 * CAKE_SPRITE_SIZE_Y));
	sprite->addKeyframe(CAKE, glm::vec2(CAKE_SPRITE_SIZE_X, 3 * CAKE_SPRITE_SIZE_Y));




	sprite->changeAnimation(true, CAKE);
}
