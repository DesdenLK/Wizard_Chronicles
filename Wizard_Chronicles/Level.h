#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE
#include "Scene.h"
class Level : public Scene
{
public:
	Level();
	~Level();

	virtual void init();
	virtual void update(int deltaTime);
	virtual void render();

	bool isLevelFinished() override;
	bool gameOverAnimationFinished() override;
	bool LevelPassedAnimationFinished() override;

protected:
	void initShaders();

protected:
	TileMap* map;
	Player* player;
	Camera* camera;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Gui* gui;
	Texture levelPassedText;
	Sprite* levelPassed;

	Texture GameOverText;
	Sprite* gameOver;

	bool levelFinished, boolGameOver;

	void setAnimations();

};

class TutorialLevel : public Level
{
	public:
		TutorialLevel();
		~TutorialLevel();

		void init() override;
		void render() override;
		int getLevel() override;
};

class Level1 : public Level
{
	public:
		Level1();
		~Level1();

		void init() override;
		void update(int deltaTime) override;
		void render() override;
		int getLevel() override;
	private:
		bool inUnderground;
};

#endif // _LEVEL_INCLUDE

