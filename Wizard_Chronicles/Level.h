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

	bool levelFinished;
};

class TutorialLevel : public Level
{
	public:
		TutorialLevel();
		~TutorialLevel();

		void init() override;
		int getLevel() override;
};

class Level1 : public Level
{
	public:
		Level1();
		~Level1();

		void init() override;
		int getLevel() override;
};

#endif // _LEVEL_INCLUDE

