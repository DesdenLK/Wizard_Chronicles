#include "DynamicObjectBox.h"
#include <iostream>



enum BoxAnims {BOX, NUM_ANIMS};


void DynamicObjectBox::init(string pathToFile, float x, float y, float w, float h, glm::vec2 quadSize, float spriteWidth, float spriteHeight, glm::vec2 offSet, ShaderProgram& shaderProgram, TileMap* map)
{
	//DynamicObject::init(pathToFile, x, y, w, h, quadSize, spriteWidth, spriteHeight, offSet, shaderProgram, map);
	posicio = glm::vec2(x, y);
	measures = glm::vec2(w, h);
	this->spriteOffset = offSet;
	this->map = map;


	objectState = { false, false };

	speed = glm::vec2(0, 0);

	spritesheet.loadFromFile(pathToFile, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(quadSize, glm::vec2(spriteWidth, spriteHeight), &spritesheet, &shaderProgram);

	sprite->setPosition(posicio);
	setAnimations();
}


void DynamicObjectBox::setAnimations()
{
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(BOX, 8);
	sprite->addKeyframe(BOX, glm::vec2(0, 0));

	sprite->changeAnimation(0);

}


