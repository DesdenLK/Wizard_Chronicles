#include "Object.h"
#include <iostream>

Object::Object(int id, float x, float y, float w, float h)
{
	this->id = id;
	posicio = glm::vec2(x, y);
	measures = glm::vec2(w, h);
}

Object::Object()
{
	posicio = glm::vec2(0, 0);
	measures = glm::vec2(0, 0);
}

Object::~Object()
{

}

glm::vec2 Object::getPosicio() const
{
	return posicio;
}

glm::vec2 Object::getMeasures() const
{
	return measures;
}

bool Object::objectCollision(glm::vec2 pos, glm::vec2 size)
{
	// separardes eix x
	if (pos.x + size.x < posicio.x || posicio.x + measures.x < pos.x)
		return false;

	// separades eix y
	if (pos.y + size.y < posicio.y || posicio.y + measures.y < pos.y)
		return false;


	return true;
}

bool Object::isPickable()
{
	return false;
}


bool Object::bottomCollision(glm::vec2 characterPos, glm::vec2 characterSize)
{
	// Coordenadas clave del personaje
	float characterTop = characterPos.y; // La parte superior del personaje
	float characterLeft = characterPos.x;
	float characterRight = characterPos.x + characterSize.x;

	// Coordenadas clave del objeto (asumiendo que posicio y measures son las del objeto)
	float objectBottom = posicio.y + measures.y; // La parte inferior del objeto
	float objectLeft = posicio.x;
	float objectRight = posicio.x + measures.x;

	// Comprobar colisión en la parte inferior del objeto
	bool verticalCollision = characterTop <= objectBottom && characterPos.y + characterSize.y > objectBottom;
	bool horizontalCollision = characterRight > objectLeft && characterLeft < objectRight;

	return verticalCollision && horizontalCollision;
}

bool Object::topCollision(glm::vec2 characterPos, glm::vec2 characterSize)
{
	// Coordenadas clave del personaje
	float characterBottom = characterPos.y + characterSize.y; // La parte inferior del personaje
	float characterLeft = characterPos.x;
	float characterRight = characterPos.x + characterSize.x;

	// Coordenadas clave del objeto (asumiendo que posicio y measures son las del objeto)
	float objectTop = posicio.y; // La parte superior del objeto
	float objectLeft = posicio.x;
	float objectRight = posicio.x + measures.x;

	// Comprobar colisión en la parte superior del objeto
	bool verticalCollision = characterBottom >= objectTop && characterPos.y < objectTop;
	bool horizontalCollision = characterRight > objectLeft && characterLeft < objectRight;

	return verticalCollision && horizontalCollision;
}


bool Object::leftCollision(glm::vec2 characterPos, glm::vec2 characterSize)
{
	// Coordenadas clave del personaje
	float characterRight = characterPos.x + characterSize.x; // El lado derecho del personaje
	float characterTop = characterPos.y;
	float characterBottom = characterPos.y + characterSize.y;

	// Coordenadas clave del objeto (asumiendo que posicio y measures son las del objeto)
	float objectLeft = posicio.x; // El lado izquierdo del objeto
	float objectTop = posicio.y;
	float objectBottom = posicio.y + measures.y;

	// Comprobar colisión en el lado izquierdo del objeto
	bool horizontalCollision = characterRight >= objectLeft && characterPos.x < objectLeft;
	bool verticalCollision = characterBottom > objectTop && characterTop < objectBottom;

	return horizontalCollision && verticalCollision;
}


bool Object::rightCollision(glm::vec2 characterPos, glm::vec2 characterSize)
{
	// Coordenadas clave del personaje
	float characterLeft = characterPos.x; // El lado izquierdo del personaje
	float characterTop = characterPos.y;
	float characterBottom = characterPos.y + characterSize.y;

	// Coordenadas clave del objeto (asumiendo que posicio y measures son las del objeto)
	float objectRight = posicio.x + measures.x; // El lado derecho del objeto
	float objectTop = posicio.y;
	float objectBottom = posicio.y + measures.y;

	// Comprobar colisión en el lado derecho del objeto
	bool horizontalCollision = characterLeft <= objectRight && characterPos.x + characterSize.x > objectRight;
	bool verticalCollision = characterBottom > objectTop && characterTop < objectBottom;

	return horizontalCollision && verticalCollision;
}


void Object::setPosicio(glm::vec2 pos)
{
	posicio = pos;
}

void Object::setMeasures(glm::vec2 size)
{
	measures = size;
}

