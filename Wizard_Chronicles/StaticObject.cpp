#include "StaticObject.h"

StaticObject::StaticObject(int id, std::string type, float x, float y, float w, float h)
{
	posicio = glm::vec2(x, y);
	measures = glm::vec2(w, h);
	this->type = type;
	this->id = id;
}

StaticObject::StaticObject()
{
	posicio = glm::vec2(0, 0);
	measures = glm::vec2(0, 0);
	type = "";
}

StaticObject::~StaticObject()
{

}

std::string StaticObject::getType() const
{
	return type;
}

