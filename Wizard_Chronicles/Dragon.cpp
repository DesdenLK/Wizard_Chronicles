#include "Dragon.h"
#include "Player.h"
#include "TileMap.h"
#include <iostream>

#define SPRITE_HEIGHT (1.f/13.f)
#define SPRITE_WIDTH (1.f/15.f)

#define HEAD_OFFSET_X 0			// x offset respecte top-left del cos del drac
#define HEAD_OFFSET_Y 11			// y offset respecte top-left del cos del drac

#define SHOOTING_TIME_DELAY 1000.f		// temps que transcorre entre 2 series de dispars de projectils (una serie acaba quan el drac ha disparat i deixa de fixar el cap)
#define RENDER_SHOOTING_TIME 1500.f		// durada de quan el drac fixa el cap a la direcció del player i dispara els projectils

#define M_PI 3.1415926535897932384626433832795

enum DragonBodyAnims
{
	FLY, NUM_ANIMS_B
};

enum DragonHeadAnims
{
	LOOK, LOOK_FIXED_LEFT, LOOK_FIXED_LEFT_DOWN, LOOK_FIXED_DOWN, LOOK_FIXED_RIGHT_DOWN, NUM_ANIMS_H
};


void Dragon::init(int id, const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string pathToSpritesheet, const glm::ivec2& spriteBodyDimensions, const glm::ivec2& spriteHeadDimensions, const glm::vec2& hitboxWH, int eraseAnimTime) {
	this->id = id;
	eraseAnimationTime = eraseAnimTime;
	boundingBoxWidthHeight = hitboxWH;
	Jumping = false;
	loopTimesInactive = 0;
	EnemyVelocity = glm::vec2(0.f,0.f);
	EnemyAcceleration = glm::vec2(0, 0);
	hitBoxEnabled = true;
	shootingTimeout = SHOOTING_TIME_DELAY;
	renderShootingTime = 0;
	dragonPhase = IDLE;

	bool loaded = spritesheet.loadFromFile(pathToSpritesheet, TEXTURE_PIXEL_FORMAT_RGBA);
	if (!loaded) cout << "texture could not load" << endl;
	else cout << "texture loaded" << endl;
	sprite = Sprite::createSprite(spriteBodyDimensions, glm::vec2(1.f/3.f, (7.f/13.f)), &spritesheet, &shaderProgram);
	dragonHead = Sprite::createSprite(spriteHeadDimensions, glm::vec2(1.f/5.f, 2.f/13.f), &spritesheet, &shaderProgram);

	setAnimations();

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	dragonHead->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x + HEAD_OFFSET_X), float(tileMapDispl.y + posEnemy.y + HEAD_OFFSET_Y)));
}

void Dragon::updateEnemyMovement(int deltaTime) {
	dragonHead->update(deltaTime);
	// Idle phase (not shooting)
	if (dragonPhase == IDLE) {
		if (shootingTimeout > 0) shootingTimeout -= deltaTime;
		else {
			dragonPhase = SHOOTING;
			glm::vec2 centerPosPlayer = player->getPosition() + glm::vec2(16, 16);
			if (centerPosPlayer.x < posEnemy.x - boundingBoxWidthHeight.x / 2.f) dragonHead->changeAnimation(true, LOOK_FIXED_LEFT);
			else if (centerPosPlayer.x < posEnemy.x) dragonHead->changeAnimation(true, LOOK_FIXED_LEFT_DOWN);
			else if (centerPosPlayer.x > posEnemy.x + boundingBoxWidthHeight.x) dragonHead->changeAnimation(true, LOOK_FIXED_RIGHT_DOWN);
			else dragonHead->changeAnimation(true, LOOK_FIXED_DOWN);

			// el primer projectil es llenca apuntant a l'objectiu (vector centerPosPlayer-dragonHeadPos), els altres dos 45 graus desviats a cada banda
			glm::vec2 playerDragonDir = centerPosPlayer - dragonHead->getPosition();
			vector<glm::vec2> projectileDirections = vector<glm::vec2>(3);

			float dispersionAngle = M_PI / 4.f;
			projectileDirections[0] = glm::normalize(playerDragonDir);
			projectileDirections[0] *= glm::vec2(2, 2);		// multiplicar per la velocitat del projectil

			projectileDirections[1] = glm::vec2(playerDragonDir.x * glm::cos(dispersionAngle) - playerDragonDir.y * glm::sin(dispersionAngle), playerDragonDir.x * glm::sin(dispersionAngle) + playerDragonDir.y * glm::cos(dispersionAngle));
			projectileDirections[1] = glm::normalize(projectileDirections[1]);
			projectileDirections[1] *= glm::vec2(1, 1);

			projectileDirections[2] = glm::vec2(playerDragonDir.x * glm::cos(-dispersionAngle) - playerDragonDir.y * glm::sin(-dispersionAngle), playerDragonDir.x * glm::sin(-dispersionAngle) + playerDragonDir.y * glm::cos(-dispersionAngle));
			projectileDirections[2] = glm::normalize(projectileDirections[2]);
			projectileDirections[2] *= glm::vec2(1, 1);

			//map->initProjectiles(projectileDirections);
			renderShootingTime = RENDER_SHOOTING_TIME;
			shootingTimeout = SHOOTING_TIME_DELAY;
		}
	}
	// Shooting phase
	else {
		if (renderShootingTime > 0) renderShootingTime -= deltaTime;
		else {
			dragonPhase = IDLE;
			renderShootingTime = 0;
			shootingTimeout = SHOOTING_TIME_DELAY;
			dragonHead->changeAnimation(true, LOOK);
		}
	}
	
}

void Dragon::setAnimations() {
	sprite->setNumberAnimations(NUM_ANIMS_B);
	dragonHead->setNumberAnimations(NUM_ANIMS_H);

	sprite->setAnimationSpeed(FLY, 4);
	sprite->addKeyframe(FLY, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(FLY, glm::vec2(5 * SPRITE_WIDTH, 0.f));
	sprite->addKeyframe(FLY, glm::vec2(10 * SPRITE_WIDTH, 0.f));
	
	dragonHead->setAnimationSpeed(LOOK, 5);
	dragonHead->addKeyframe(LOOK, glm::vec2(0.f, 7 * SPRITE_HEIGHT));
	dragonHead->addKeyframe(LOOK, glm::vec2(3 * SPRITE_WIDTH, 7 * SPRITE_HEIGHT));
	dragonHead->addKeyframe(LOOK, glm::vec2(3 * SPRITE_WIDTH, 9 * SPRITE_HEIGHT));
	dragonHead->addKeyframe(LOOK, glm::vec2(0.f, 9 * SPRITE_HEIGHT));

	dragonHead->setAnimationSpeed(LOOK_FIXED_LEFT, 4);
	dragonHead->addKeyframe(LOOK_FIXED_LEFT, glm::vec2(0.f, 7 * SPRITE_HEIGHT));

	dragonHead->setAnimationSpeed(LOOK_FIXED_LEFT_DOWN, 4);
	dragonHead->addKeyframe(LOOK_FIXED_LEFT_DOWN, glm::vec2(3 * SPRITE_WIDTH, 7 * SPRITE_HEIGHT));

	dragonHead->setAnimationSpeed(LOOK_FIXED_DOWN, 4);
	dragonHead->addKeyframe(LOOK_FIXED_DOWN, glm::vec2(3 * SPRITE_WIDTH, 9 * SPRITE_HEIGHT));

	dragonHead->setAnimationSpeed(LOOK_FIXED_RIGHT_DOWN, 4);
	dragonHead->addKeyframe(LOOK_FIXED_RIGHT_DOWN, glm::vec2(0.f, 9 * SPRITE_HEIGHT));

	sprite->changeAnimation(true, FLY);
	dragonHead->changeAnimation(true, LOOK);
}

void Dragon::render() {
	cout << "rendering dragon" << endl;
	sprite->render();
	dragonHead->render();
	cout << "dragon body pos: " << '(' << sprite->getPosition().x << ',' << sprite->getPosition().y << ')' << endl;
	cout << "dragon head pos: " << '(' << dragonHead->getPosition().x << ',' << dragonHead->getPosition().y << ')' << endl;
	cout << "dragon rendered" << endl;
}

void Dragon::setPosition(const glm::vec2& pos) {
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	dragonHead->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x + HEAD_OFFSET_X), float(tileMapDispl.y + posEnemy.y + HEAD_OFFSET_Y)));
}

void Dragon::setPlayer(Player* player) {
	this->player = player;
}

void Dragon::changeToDeadAnimation() {
	return;
}

int Dragon::getEraseAnimationTime() {
	return 1000;
}