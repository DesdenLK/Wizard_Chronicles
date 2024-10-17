#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

glm::mat4 Camera::init(glm::vec2 pos, int width, int height)
{
	cameraPos = pos;
	CAMERA_WIDTH = width;
	CAMERA_HEIGHT = height;

	return glm::ortho(float(cameraPos.x), float(cameraPos.x + CAMERA_WIDTH), float(cameraPos.y + CAMERA_HEIGHT), float(cameraPos.y));
}

glm::mat4 Camera::cameraPositionNOCENTRAT(glm::vec2 posPlayer)
{

	if ((posPlayer.x - cameraPos.x) < CAMERA_WIDTH / 3) {
		cameraPos.x = posPlayer.x - CAMERA_WIDTH / 3;
	}
	if ((posPlayer.x - cameraPos.x) > (2 * CAMERA_WIDTH / 3)) {
		cameraPos.x = posPlayer.x - (2 * CAMERA_WIDTH / 3);
	}

	if ((posPlayer.y - cameraPos.y) < CAMERA_HEIGHT / 3) {
		cameraPos.y = posPlayer.y - CAMERA_HEIGHT / 3;
	}
	if ((posPlayer.y - cameraPos.y) > (2 * CAMERA_HEIGHT / 3)) {
		cameraPos.y = posPlayer.y - (2 * CAMERA_HEIGHT / 3);
	}
	return glm::ortho(float(cameraPos.x), float(cameraPos.x + CAMERA_WIDTH), float(cameraPos.y + CAMERA_HEIGHT), float(cameraPos.y));
}

glm::mat4 Camera::cameraPositionCENTRAT(glm::vec2 posPlayer, glm::vec2 playerV)
{

	//POSICIO PER X
	int cameraVx = 0;

	if (playerV.x > 0) {
		if ((posPlayer.x - cameraPos.x) > int(CAMERA_WIDTH / 3))
		{
			cameraVx = 2 * playerV.x;

		}


	}
	else {
		if ((posPlayer.x - cameraPos.x) < int(2 * CAMERA_WIDTH / 3))
		{
			cameraVx = 2 * playerV.x;
		}
	}

	int cameraVy = 0;
	if (playerV.y > 0)
	{
		if ((posPlayer.y - cameraPos.y) > int(CAMERA_HEIGHT / 3))
		{
			cameraVy = 2 * playerV.y;
		}
	}
	else
	{
		if ((posPlayer.y - cameraPos.y) < int(2 * CAMERA_HEIGHT / 3))
		{
			cameraVy = 2 * playerV.y;
		}
	}

	cameraPos.x += cameraVx;
	cameraPos.y += cameraVy;
	//std::cout << "POSPLAYER 1: " << posPlayer.x - 2 * CAMERA_WIDTH / 3 << std::endl;
	cameraPos.x = std::max(cameraPos.x, posPlayer.x - 2 * CAMERA_WIDTH / 3);
	cameraPos.y = std::max(cameraPos.y, posPlayer.y - 2 * CAMERA_HEIGHT / 3);
	//std::cout << "POSPLAYER 2: " << cameraPos.x << std::endl;
	cameraPos.x = std::min(cameraPos.x, posPlayer.x - CAMERA_WIDTH / 3);
	cameraPos.y = std::min(cameraPos.y, posPlayer.y - CAMERA_HEIGHT / 3);
	//std::cout << "MIN :" << cameraPos.x << std::endl;

	return glm::ortho(float(cameraPos.x), float(cameraPos.x + CAMERA_WIDTH), float(cameraPos.y + CAMERA_HEIGHT), float(cameraPos.y));
}

glm::mat4 Camera::cameraPositionMix(glm::vec2 posPlayer, glm::vec2 playerV)
{
	//POSICIO PER X
	int cameraVx = 0;

	if (playerV.x > 0) {
		if ((posPlayer.x - cameraPos.x) > int(CAMERA_WIDTH / 3))
		{
			cameraVx = 2 * playerV.x;

		}


	}
	else {
		if ((posPlayer.x - cameraPos.x) < int(2 * CAMERA_WIDTH / 3))
		{
			cameraVx = 2 * playerV.x;
		}
	}

	cameraPos.x += cameraVx;
	cameraPos.x = std::max(cameraPos.x, posPlayer.x - 2 * CAMERA_WIDTH / 3);
	cameraPos.x = std::min(cameraPos.x, posPlayer.x - CAMERA_WIDTH / 3);


	if ((posPlayer.y - cameraPos.y) < CAMERA_HEIGHT / 3) {
		cameraPos.y = posPlayer.y - CAMERA_HEIGHT / 3;
	}
	if ((posPlayer.y - cameraPos.y) > (2 * CAMERA_HEIGHT / 3)) {
		cameraPos.y = posPlayer.y - (2 * CAMERA_HEIGHT / 3);
	}
	return glm::ortho(float(cameraPos.x), float(cameraPos.x + CAMERA_WIDTH), float(cameraPos.y + CAMERA_HEIGHT), float(cameraPos.y));
}