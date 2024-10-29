#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"

#include "Level.h"
#include "StartScene.h"
#include "MainMenu.h"
#include "Credit.h"

#include <iostream>

void Game::init()
{
	bPlay = true;
	tries = 3;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	startScene = true;
	credits, level, mainMenu = false;

	scene = new StartScene();
	scene -> init();
}
 
bool Game::update(int deltaTime)
{
	if (getKey(GLFW_KEY_ENTER) and startScene and not enterPressed) {
		startScene = false;
		mainMenu = true;
		delete scene;
		scene = new MainMenu();
		scene->init();
		enterPressed = true;
	}

	else if (getKey(GLFW_KEY_ENTER) and mainMenu and not enterPressed and scene->getLevel() != -1) {
		mainMenu = false;
		level = true;
		int level = scene->getLevel();
		delete scene;
		if (level  == 0) scene = new TutorialLevel();
		else if (level  == 1) scene = new Level1();
		else scene = new MainMenu();
		scene->init();
		enterPressed = true;
	}

	else if (level and scene -> isLevelFinished()) {
		level = false;
		if (scene->getLevel() == 0) {
			delete scene;
			mainMenu = true;
			scene = new MainMenu();
		}
		else {
			delete scene;
			credits = true;
			scene = new Credit();
		}
		scene->init();
	}

	else if (credits and getKey(GLFW_KEY_ENTER) and not enterPressed) {
		bPlay = false;
	}

	if (not getKey(GLFW_KEY_ENTER) and enterPressed) enterPressed = false;

	if (tries == 0) bPlay = false;
	//std::cout << "Tries: " << tries << endl;
	scene -> update(deltaTime);

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene -> render();
}

void Game::keyPressed(int key)
{
	if(key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

void Game::setTries(int t)
{
	tries = t;
}

int Game::getTries() const
{
	return tries;
}



