#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Camera.h"
#include "Gui.h"



// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	virtual void init();
	virtual void update(int deltaTime);
	virtual void render();

	virtual bool inInstructions();
	virtual int getLevel();

	virtual bool isLevelFinished();
	virtual bool gameOverAnimationFinished();
	virtual bool LevelPassedAnimationFinished();

private:
	void initShaders();

protected:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _SCENE_INCLUDE

