#include "GemPickable.h"
#include "TileMap.h"

void GemPickable::init(int id, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, ShaderProgram& shaderProgram, TileMap* map) {
	this->id = id;
	posicio = glm::vec2(x, y);
	measures = glm::vec2(w, h);
	speed = glm::vec2(0, 0);

	jumpAngle = 0;
	startY = posicio.y;
	objectState = { true, false };

	this->map = map;



	posicio = glm::vec2(x, y);
	measures = glm::vec2(w, h);

	spritesheet.loadFromFile("images/DragonGem.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(quadSize, glm::vec2(spriteWidth, spriteHeight), &spritesheet, &shaderProgram);

	sprite->setPosition(posicio);
}

string GemPickable::getType() {
	return "Gem";
}

