#include "Level.h"

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


Level::Level()
{
	map = NULL;
	player = NULL;
	camera = NULL;
}

Level::~Level()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (camera != NULL)
		delete camera;
}


void Level::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/Luca.tmj", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, 200 * 1000);
	player = new Player();
	camera = new Camera();

	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	projection = camera->init(glm::vec2(0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	currentTime = 0.0f;

	gui = new Gui();
	glm::vec2 pos = camera->getCameraPos();
	gui->init(pos, glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), 1, 1, texProgram);

	levelFinished = false;
}

void Level::update(int deltaTime)
{
	if (player->getPlayerLifes() == 0) { init(); Game::instance().setTries(Game::instance().getTries() - 1); }
	if (map->getTimeLeft() <= 0) { init(); Game::instance().setTries(Game::instance().getTries() - 1); }

	//cout << "Lifes: " << player->getPlayerLifes() << endl;
	currentTime += deltaTime;
	map->update(deltaTime);
	player->update(deltaTime);


	gui->update(deltaTime, player->getPlayerLifes(), Game::instance().getTries(), map->getPlayerScore(), map->getTimeLeft() / 1000);

	if (Game::instance().getKey(GLFW_KEY_F)) levelFinished = true;
}

void Level::render()
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

	glm::mat4 guiProjection = glm::ortho(0.0f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.0f);
	texProgram.setUniformMatrix4f("projection", guiProjection);
	texProgram.setUniformMatrix4f("modelview", modelview);
	gui->render();

}

bool Level::isLevelFinished()
{
	return levelFinished;
}


void Level::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

TutorialLevel::TutorialLevel()
{
	map = NULL;
	player = NULL;
	camera = NULL;
}

TutorialLevel::~TutorialLevel()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (camera != NULL)
		delete camera;
}

void TutorialLevel::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/Luca.tmj", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, 200 * 1000);
	player = new Player();
	camera = new Camera();

	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	projection = camera->init(glm::vec2(0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	currentTime = 0.0f;

	gui = new Gui();
	glm::vec2 pos = camera->getCameraPos();
	gui->init(pos, glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), 1, 1, texProgram);

	levelFinished = false;
}

int TutorialLevel::getLevel()
{
	return 0;
}

Level1::Level1()
{
	map = NULL;
	player = NULL;
	camera = NULL;
}

Level1::~Level1()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (camera != NULL)
		delete camera;
}

void Level1::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/Luca.tmj", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, 200 * 1000);
	player = new Player();
	camera = new Camera();

	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	projection = camera->init(glm::vec2(0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	currentTime = 0.0f;

	gui = new Gui();
	glm::vec2 pos = camera->getCameraPos();
	gui->init(pos, glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), 1, 1, texProgram);

	levelFinished = false;
}

int Level1::getLevel()
{
	return 1;
}
