#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 0
#define INIT_PLAYER_Y_TILES 3


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}



void Scene::init()
{
	initShaders();
	cameraPos = glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	map = TileMap::createTileMap("levels/proba.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview;
	cameraPositionMix();


	
	projection = glm::ortho(float(cameraPos.x), float(cameraPos.x + SCREEN_WIDTH), float(cameraPos.y + SCREEN_HEIGHT), float(cameraPos.y));
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
}

void Scene::cameraPositionNOCENTRAT()
{
	glm::vec2 posPlayer = player->getPosition();
	glm::vec2 playerV = player->getVelocity();

	if ((posPlayer.x - cameraPos.x) < SCREEN_WIDTH / 3) {
		cameraPos.x = posPlayer.x - SCREEN_WIDTH / 3;
	}
	if ((posPlayer.x - cameraPos.x) > (2*SCREEN_WIDTH / 3)) {
		cameraPos.x = posPlayer.x - (2*SCREEN_WIDTH / 3);
	}

	if ((posPlayer.y - cameraPos.y) < SCREEN_HEIGHT / 3) {
		cameraPos.y = posPlayer.y - SCREEN_HEIGHT / 3;
	}
	if ((posPlayer.y - cameraPos.y) > (2 * SCREEN_HEIGHT / 3)) {
		cameraPos.y = posPlayer.y - (2 * SCREEN_HEIGHT / 3);
	}

}

void Scene::cameraPositionCENTRAT()
{
	glm::vec2 posPlayer = player->getPosition();
	glm::vec2 playerV = player->getVelocity();


	//POSICIO PER X
	int cameraVx = 0;

	if (playerV.x > 0) {
		if ((posPlayer.x - cameraPos.x) > int(SCREEN_WIDTH / 3))
		{
			cameraVx = 2 * playerV.x;

		}


	}
	else {
		if ((posPlayer.x - cameraPos.x) < int(2 * SCREEN_WIDTH / 3))
		{
			cameraVx = 2 * playerV.x;
		}
	}

	int cameraVy = 0;
	if (playerV.y > 0)
	{
		if ((posPlayer.y - cameraPos.y) > int(SCREEN_HEIGHT / 3))
		{
			cameraVy = 2 * playerV.y;
		}
	}
	else
	{
		if ((posPlayer.y - cameraPos.y) < int(2 * SCREEN_HEIGHT / 3))
		{
			cameraVy = 2 * playerV.y;
		}
	}

	cameraPos.x += cameraVx;
	cameraPos.y += cameraVy;
	//std::cout << "POSPLAYER 1: " << posPlayer.x - 2 * SCREEN_WIDTH / 3 << std::endl;
	cameraPos.x = std::max(cameraPos.x, posPlayer.x - 2 * SCREEN_WIDTH / 3);
	cameraPos.y = std::max(cameraPos.y, posPlayer.y - 2 * SCREEN_HEIGHT / 3);
	//std::cout << "POSPLAYER 2: " << cameraPos.x << std::endl;
	cameraPos.x = std::min(cameraPos.x, posPlayer.x - SCREEN_WIDTH / 3);
	cameraPos.y = std::min(cameraPos.y, posPlayer.y - SCREEN_HEIGHT / 3);
	//std::cout << "MIN :" << cameraPos.x << std::endl;
}

void Scene::cameraPositionMix()
{
	glm::vec2 posPlayer = player->getPosition();
	glm::vec2 playerV = player->getVelocity();


	//POSICIO PER X
	int cameraVx = 0;

	if (playerV.x > 0) {
		if ((posPlayer.x - cameraPos.x) > int(SCREEN_WIDTH / 3))
		{
			cameraVx = 2 * playerV.x;

		}


	}
	else {
		if ((posPlayer.x - cameraPos.x) < int(2 * SCREEN_WIDTH / 3))
		{
			cameraVx = 2 * playerV.x;
		}
	}

	cameraPos.x += cameraVx;
	cameraPos.x = std::max(cameraPos.x, posPlayer.x - 2 * SCREEN_WIDTH / 3);
	cameraPos.x = std::min(cameraPos.x, posPlayer.x - SCREEN_WIDTH / 3);


	if ((posPlayer.y - cameraPos.y) < SCREEN_HEIGHT / 3) {
		cameraPos.y = posPlayer.y - SCREEN_HEIGHT / 3;
	}
	if ((posPlayer.y - cameraPos.y) > (2 * SCREEN_HEIGHT / 3)) {
		cameraPos.y = posPlayer.y - (2 * SCREEN_HEIGHT / 3);
	}

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



