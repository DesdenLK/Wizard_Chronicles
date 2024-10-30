#include "Level.h"

#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "CaterpillarEnemy.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES	140	//7
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

	map->setPlayer(player);

	gui = new Gui();
	glm::vec2 pos = camera->getCameraPos();
	gui->init(pos, glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), 1, 1, texProgram);

	levelPassedText.loadFromFile("images/levelPassed.png", TEXTURE_PIXEL_FORMAT_RGBA);
	levelPassed = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(0.2, 1), &levelPassedText, &texProgram);

	GameOverText.loadFromFile("images/GameOver.png", TEXTURE_PIXEL_FORMAT_RGBA);
	gameOver = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(0.2, 1), &GameOverText, &texProgram);

	setAnimations();
	levelFinished = false;
}

void Level::update(int deltaTime)
{
	if (not boolGameOver) {
		if (player->getPlayerLifes() == 0) { init(); Game::instance().setTries(Game::instance().getTries() - 1); }
		if (map->getTimeLeft() <= 0) { init(); Game::instance().setTries(Game::instance().getTries() - 1); }

		//cout << "Lifes: " << player->getPlayerLifes() << endl;
		currentTime += deltaTime;

		player->update(deltaTime);
		map->update(deltaTime);

		gui->update(deltaTime, player->getPlayerLifes(), Game::instance().getTries(), map->getPlayerScore(), map->getTimeLeft() / 1000);


		if (not levelFinished) levelFinished = map->isGemObtained();
		if (Game::instance().getKey(GLFW_KEY_F)) levelFinished = true;
		if (levelFinished) levelPassed->update(deltaTime);


		boolGameOver = Game::instance().getTries() == 0;

	}

	else {
		gameOver->update(deltaTime);
	}
		

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
	if (boolGameOver) gameOver->render();
	else if (levelFinished) levelPassed->render();
	gui->render();

}

bool Level::isLevelFinished()
{
	return levelFinished;
}

bool Level::gameOverAnimationFinished()
{
	return gameOver->isAnimationFinished();
}

bool Level::LevelPassedAnimationFinished()
{
	return levelPassed->isAnimationFinished();
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

void Level::setAnimations()
{
	levelPassed->setNumberAnimations(1);
	gameOver->setNumberAnimations(1);

	levelPassed->setAnimationSpeed(0, 2);
	levelPassed->addKeyframe(0, glm::vec2(0.f, 0.f));
	levelPassed->addKeyframe(0, glm::vec2(0.2f, 0.f));
	levelPassed->addKeyframe(0, glm::vec2(0.4f, 0.f));
	levelPassed->addKeyframe(0, glm::vec2(0.6f, 0.f));
	levelPassed->addKeyframe(0, glm::vec2(0.8f, 0.f));

	gameOver->setAnimationSpeed(0, 8);
	gameOver->addKeyframe(0, glm::vec2(0.f, 0.f));
	gameOver->addKeyframe(0, glm::vec2(0.2f, 0.f));
	gameOver->addKeyframe(0, glm::vec2(0.4f, 0.f));
	gameOver->addKeyframe(0, glm::vec2(0.6f, 0.f));
	gameOver->addKeyframe(0, glm::vec2(0.8f, 0.f));

	levelPassed->changeAnimation(false, 0);
	gameOver->changeAnimation(false, 0);


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
	map = TileMap::createTileMap("levels/Tutorial.tmj", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, 200 * 1000);
	player = new Player();
	camera = new Camera();

	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(7 * map->getTileSize(), 19 * map->getTileSize()));
	player->setTileMap(map);
	projection = camera->init(glm::vec2(0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	currentTime = 0.0f;

	gui = new Gui();
	glm::vec2 pos = camera->getCameraPos();
	gui->init(pos, glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), 1, 1, texProgram);

	levelPassedText.loadFromFile("images/levelPassed.png", TEXTURE_PIXEL_FORMAT_RGBA);
	levelPassed = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(0.2, 1), &levelPassedText, &texProgram);

	GameOverText.loadFromFile("images/GameOver.png", TEXTURE_PIXEL_FORMAT_RGBA);
	gameOver = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(0.2, 1), &GameOverText, &texProgram);

	levelFinished = false;

	setAnimations();
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
	map = TileMap::createTileMap("levels/Level1.tmj", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, 200 * 1000);
	player = new Player();
	camera = new Camera();

	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	projection = camera->init(glm::vec2(0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	currentTime = 0.0f;

	map->setPlayer(player);

	gui = new Gui();
	glm::vec2 pos = camera->getCameraPos();
	gui->init(pos, glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), 1, 1, texProgram);

	levelPassedText.loadFromFile("images/levelPassed.png", TEXTURE_PIXEL_FORMAT_RGBA);
	levelPassed = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(0.2, 1), &levelPassedText, &texProgram);

	GameOverText.loadFromFile("images/GameOver.png", TEXTURE_PIXEL_FORMAT_RGBA);
	gameOver = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(0.2, 1), &GameOverText, &texProgram);

	levelFinished = false;

	setAnimations();
}

void Level1::update(int deltaTime) {
	if (not boolGameOver) {
		if (player->getPlayerLifes() == 0) { init(); Game::instance().setTries(Game::instance().getTries() - 1); }
		if (map->getTimeLeft() <= 0) { init(); Game::instance().setTries(Game::instance().getTries() - 1); }

		//cout << "Lifes: " << player->getPlayerLifes() << endl;
		currentTime += deltaTime;

		float playerY = player->getPosition().y;
		if (not inUnderground and playerY > 14 and playerY < 33) {		//player esta al underground
			camera->cameraPositionCENTRAT(player->getPosition(), player->getVelocity());
			inUnderground = true;
		}
		else if ((playerY < 14 or playerY > 33) and inUnderground) {
			camera->cameraPositionNOCENTRAT(player->getPosition());
		}
		player->update(deltaTime);
		map->update(deltaTime);

		gui->update(deltaTime, player->getPlayerLifes(), Game::instance().getTries(), map->getPlayerScore(), map->getTimeLeft() / 1000);


		if (not levelFinished) levelFinished = map->isGemObtained();
		if (Game::instance().getKey(GLFW_KEY_F)) levelFinished = true;
		if (levelFinished) levelPassed->update(deltaTime);


		boolGameOver = Game::instance().getTries() == 0;

	}

	else {
		gameOver->update(deltaTime);
	}
}

int Level1::getLevel()
{
	return 1;
}
