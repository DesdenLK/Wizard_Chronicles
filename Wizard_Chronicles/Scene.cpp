#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "CaterpillarEnemy.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 14
#define INIT_PLAYER_Y_TILES 10


Scene::Scene()
{
}

Scene::~Scene()
{
}


void Scene::init()
{
	initShaders();
	projection = glm::ortho(0.0f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.0f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	return;
}

void Scene::render()
{
	return;
}

bool Scene::inInstructions()
{
	return false;
}

int Scene::getLevel()
{
	return -1;
}

bool Scene::isLevelFinished()
{
	return false;
}


void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



