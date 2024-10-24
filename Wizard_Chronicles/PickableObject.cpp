#include "PickableObject.h"
#include "TileMap.h"

void PickableObject::init(int id, std::string pathToFile, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, ShaderProgram& shaderProgram, TileMap* map)
{
	this->id = id;
	posicio = glm::vec2(x, y);
	measures = glm::vec2(w, h);

	this->map = map;



	spritesheet.loadFromFile(pathToFile, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(quadSize, glm::vec2(spriteWidth, spriteHeight), &spritesheet, &shaderProgram);

	sprite->setPosition(posicio);
}

void PickableObject::render()
{
	sprite->render();
}

void PickableObject::update(int deltaTime)
{
	sprite->update(deltaTime);
}


bool PickableObject::isPickable()
{
	return true;
}
