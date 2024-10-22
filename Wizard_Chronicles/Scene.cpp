#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "CaterpillarEnemy.h"


#define SCREEN_X 0
#define SCREEN_Y 16

#define INIT_Player_X_TILES 20
#define INIT_Player_Y_TILES 14


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
	map = TileMap::createTileMap("levels/prueba2.tmj", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	vector<pair<glm::vec2, glm::vec2>> enemyBoundingBoxes = map->getEnemyBoundingBoxes();
	player = new Player();
	camera = new Camera();
	Enemies = vector<Enemy*>();
	CaterpillarEnemy* cp = new CaterpillarEnemy();
	/*for (auto boundingBox : enemyBoundingBoxes) {
		
	}*/
	cp->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "C:\\Users\\albert\\Desktop\\UNI\\Compu\\VJ\\WizardChronicles\\Wizard_Chronicles\\images\\enemics\\caterpillar\\spriteCaterpillarAmplifiedAlpha.png", glm::ivec2(25,25), glm::vec2((1.f/6.f), 1.f));
	cp->setPosition(enemyBoundingBoxes[0].first);
	//cout << "position of caterpillar: " << '(' << enemyBoundingBoxes[0].first[0] << ',' << enemyBoundingBoxes[0].first[1] << ')' << endl;
	cp->setTileMap(map);
	Enemies.push_back(cp);
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_Player_X_TILES * map->getTileSize(), INIT_Player_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	projection = camera->init(glm::vec2(0,0), SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	// per tots els enemics fer update
	for (auto& enemy : Enemies) {
		enemy->update(deltaTime);
	}
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
	map->render();
	player->render();
	//render enemics
	for (auto& enemy : Enemies) {
		enemy->render();
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



