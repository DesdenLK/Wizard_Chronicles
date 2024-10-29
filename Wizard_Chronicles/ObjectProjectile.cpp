#include "ObjectProjectile.h"
#include "TileMap.h"


#define SPRITE_WIDTH 1.f/15.f
#define SPRITE_HEIGHT 1.f/13.f

enum ProjectileAnims
{
	ALIVE, DESTROY, NUM_ANIMS
};

void ObjectProjectile::init(int id, string pathToFile, glm::vec2 startPos, glm::vec2 quadSize, ShaderProgram& shaderProgram, TileMap* map, const glm::vec2& velocityDir)
{
	this->id = id;
	this->posicio = startPos;
	this->measures = quadSize;
	this->velocityDir = velocityDir;
	this->map = map;

	loopTimesDestroy = 0;

	spritesheet.loadFromFile(pathToFile, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(quadSize, glm::vec2(SPRITE_WIDTH, SPRITE_HEIGHT), &spritesheet, &shaderProgram);

	sprite->setPosition(posicio);
	setAnimations();
}

glm::vec2 ObjectProjectile::getPosition() {
	return posicio;
}

glm::vec2 ObjectProjectile::getBoundingBoxWH() {
	return measures;
}

void ObjectProjectile::update(int deltaTime) {
	sprite->update(deltaTime);
	posicio += velocityDir;
	float posY = posicio.y + measures.y;
	if (map->collisionMoveDown(posicio, measures, &posY) or map->collisionMoveLeft(posicio, measures) or map->collisionMoveRight(posicio, measures) and not collided) {
		destroyObject();
		collided = true;
	}
	sprite->setPosition(posicio);
	//cout << "X: " << posicio.x << " Y: " << posicio.y << endl;
}

bool ObjectProjectile::hasCollided() {
	return collided;
}

void ObjectProjectile::render() {
	sprite->render();
}

void ObjectProjectile::setAnimations()
{
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(ALIVE, 4);
	sprite->addKeyframe(ALIVE, glm::vec2(0.f, 11 * SPRITE_HEIGHT));
	sprite->addKeyframe(ALIVE, glm::vec2(SPRITE_WIDTH, 11 * SPRITE_HEIGHT));
	sprite->addKeyframe(ALIVE, glm::vec2(2 * SPRITE_WIDTH, 11 * SPRITE_HEIGHT));

	sprite->setAnimationSpeed(DESTROY, 6);
	sprite->addKeyframe(DESTROY, glm::vec2(SPRITE_WIDTH, 12 * SPRITE_HEIGHT));
	sprite->addKeyframe(DESTROY, glm::vec2(0.f, 12 * SPRITE_HEIGHT));

	sprite->changeAnimation(true, ALIVE);
}

void ObjectProjectile::destroyObject() {
	sprite->changeAnimation(false, DESTROY);
	velocityDir = glm::vec2(0.f, 0.f);
}
