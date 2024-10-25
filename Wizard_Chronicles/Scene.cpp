#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "CaterpillarEnemy.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 21  //branca enemics 20
#define INIT_PLAYER_Y_TILES 14


Scene::Scene()
{
	map = NULL;
	player = NULL;
	camera = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (camera != NULL)
		delete camera;
}



void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/Luca.tmj", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, 50 * 1000);
	player = new Player();
	camera = new Camera();
	
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	projection = camera->init(glm::vec2(0,0), SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	if (player->getPlayerLifes() == 0) { init(); Game::instance().setTries(Game::instance().getTries() - 1);  }
	if (map->getTimeLeft() <= 0) { init(); Game::instance().setTries(Game::instance().getTries() - 1); }

	cout << "Lifes: " << player->getPlayerLifes() << endl;
	currentTime += deltaTime;
	map->update(deltaTime);
	player->update(deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview;


	
	projection = camera->cameraPositionNOCENTRAT(player->getPosition());
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	//map->renderDynamicObjects();
	map->render();
	player->render();

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



