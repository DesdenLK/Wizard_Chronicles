#include "Gui.h"

void Gui::init(glm::vec2 position, glm::vec2 quadSize, float spriteWidth, float spriteHeight, ShaderProgram& shaderProgram)
{

	spritesheet.loadFromFile("images/Gui.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(quadSize, glm::vec2(spriteWidth, spriteHeight), &spritesheet, &shaderProgram);

	sprite->setPosition(position);

}

void Gui::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Gui::render()
{
	sprite->render();
}
