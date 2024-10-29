#include "GemPickable.h"
#include "TileMap.h"

void GemPickable::init(int id, string pathToFile, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, ShaderProgram& shaderProgram, TileMap* map) {
	this->id = id;
	objectState.Jumping = false;
	objectState.Falling = true;
	jumpAngle = JUMP_ANGLE_STEP_PICKOBJ;
	startY = JUMP_HEIGHT_PICKOBJ;

	posicio = glm::vec2(x, y);
	measures = glm::vec2(w, h);

	spritesheet.loadFromFile(pathToFile, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(quadSize, glm::vec2(spriteWidth, spriteHeight), &spritesheet, &shaderProgram);

	sprite->setPosition(posicio);
}

string GemPickable::getType() {
	return "";
}

void GemPickable::objectFalling() {
	PickableObject::objectFalling();
}

void GemPickable::objectJump() {
	PickableObject::objectJump();
}