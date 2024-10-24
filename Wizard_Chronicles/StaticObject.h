#ifndef _STATICOBJECT_INCLUDE
#define _STATICOBJECT_INCLUDE
#include "Object.h";

class StaticObject : public Object {
public:
	StaticObject(int id, std::string type, float x, float y, float w, float h);
	StaticObject();
	~StaticObject();

	std::string getType() const;

private:
	std::string type;
};

#endif // !_STATICOBJECT_INCLUDE
