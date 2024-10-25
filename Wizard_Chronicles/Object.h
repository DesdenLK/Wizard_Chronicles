#ifndef _OBJECT_INCLUDE
#define _OBJECT_INCLUDE
#include <string>
#include <glm/glm.hpp>


class Object {

protected:
	int id;
	glm::vec2 posicio;
	glm::vec2 measures;

public:
	Object(int id, float x, float y, float w, float h);
	Object();
	~Object();

	glm::vec2 getPosicio() const;
	glm::vec2 getMeasures() const;

	bool objectCollision(glm::vec2 pos, glm::vec2 size);

	virtual bool isPickable();

	bool leftCollision(glm::vec2 pos, glm::vec2 size);
	bool rightCollision(glm::vec2 pos, glm::vec2 size);
	bool topCollision(glm::vec2 pos, glm::vec2 size);
	bool bottomCollision(glm::vec2 pos, glm::vec2 size);

	void setPosicio(glm::vec2 pos);
	void setMeasures(glm::vec2 size);
};

#endif // _OBJECT_INCLUDE
