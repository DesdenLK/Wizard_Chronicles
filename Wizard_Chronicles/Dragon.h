#ifndef _Dragon_INCLUDE
#define _Dragon_INCLUDE

#include "Enemy.h"

class Player;

enum Phase
{
	IDLE, SHOOTING
};

class Dragon :
    public Enemy
{
public:
	void init(int id, const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string pathToSpritesheet, const glm::ivec2& spriteBodyDimensions, const glm::ivec2& spriteHeadDimensions, const glm::vec2& hitboxWH, int eraseAnimTime);
	void update(int deltaTime);
	void render();
	void setPlayer(Player* player);
	void setPosition(const glm::vec2& pos);
	bool takeDamage(int damage);

	void updateEnemyMovement(int deltaTime) override;
	void setAnimations() override;
	void changeToDeadAnimation() override;
	int getEraseAnimationTime() override;

private:
	Sprite* dragonHead;
	Player* player;
	float shootingTimeout;
	float renderShootingTime;
	Phase dragonPhase;
	int lifes, hurtTime, hurtFrameTime;
	bool isHurt;
};

#endif