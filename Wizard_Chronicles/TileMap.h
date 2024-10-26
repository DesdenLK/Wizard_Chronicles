#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <json.hpp>

#include "StaticObject.h"
#include "CaterpillarEnemy.h"
#include "BeeEnemy.h"
#include "DynamicObjectChest.h"
#include "DynamicObjectBox.h"

#include "Cake.h"
#include "Coin.h"
#include <map>



// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

class DynamicObject;
class Enemy;

class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, int timeLimit);

	~TileMap();

	void render() const;
	void update(int deltaTime);
	void free();
	
	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::vec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::vec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::vec2 &pos, const glm::ivec2 &size, float *posY) const;
	bool collisionMoveUp(const glm::vec2& pos, const glm::ivec2& size, float* posY) const;

	bool ladderCollision(const glm::vec2& pos, const glm::vec2& size);
	bool isOnLadderTop(const glm::vec2& posPlayer, const glm::vec2& playerSize);
	bool isOnLadderBottom(const glm::vec2& posPlayer, const glm::vec2& playerSize);
  
	int enemyCollision(const glm::vec2& pos, const glm::vec2& size);
	int lateralCollisionWithEnemy(const glm::vec2& posPlayer, const glm::vec2& playerSize);
	int verticalCollisionWithEnemy(const glm::vec2& posPlayer, const glm::vec2& playerSize);
	void eraseEnemy(int enemyId);

	int pickingObject(const glm::vec2& posPlayer, const glm::vec2& playerSize);
	void addPickableObject(string type, glm::vec2 pos, glm::vec2 measures);
	int collisionWithPickableObject(const glm::vec2& posPlayer, const glm::vec2& playerSize);
	void erasePickableObject(int pickableObjectId);

	int collisionWithChest(const glm::vec2& posPlayer, const glm::vec2& playerSize);
	DynamicObject* getDynamicObject(int index);
	void destroyDynamicObject(int index);

	bool collisionWithInvisibleObject(const glm::vec2& pos, const glm::vec2& size);

	void renderDynamicObjects();

	void setPlayerScore(int score);
	int getPlayerScore();
	void setTimeLeft(int time);
	int getTimeLeft();

	string getPickableObjectType(int index);

	//void addPickableObject(PickableObject* object);
	
private:
	bool loadLevel(const string &levelFile, ShaderProgram& program);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
	void addTileVertices(vector<float>& vertices, const glm::vec2& posTile, const glm::vec2 texCoordTile[2]);
	void createVAO(GLuint& vao, GLuint& vbo, const std::vector<float>& vertices, ShaderProgram& program);

	void initDynamicObjects(const nlohmann::json& j, ShaderProgram& program);
	//bool boundingBoxCollision(glm::vec2 coordsMin1, glm::vec2 widthHeight1, glm::vec2 coordsMin2, glm::vec2 widthHeight2);
	bool lateralBoxCollision(glm::vec2 coordsMin1, glm::vec2 widthHeight1, glm::vec2 coordsMin2, glm::vec2 widthHeight2);
	bool verticalBoxCollision(glm::vec2 coordsMin1, glm::vec2 widthHeight1, glm::vec2 coordsMin2, glm::vec2 widthHeight2);

private:
	GLuint vaoBackground, vaoMiddle, vaoForeground;
	GLuint vboBackground, vboMiddle, vboForeground;
	GLint posLocation, texCoordLocation;
	ShaderProgram *program;

	int nBackgroundTiles, nMiddleTiles, nForegroundTiles;

	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;

	int* map, * background, * middle, * foreground;
	
	vector<StaticObject*> staticObjects;
	int nStaticObjects;

	vector<DynamicObject*> dynamicObjects;
	int nDynamicObjects;

	vector<PickableObject*> pickableObjects;
	int nPickableObjects;

	vector<StaticObject*> stairs;
	int nStairs;

	vector<StaticObject*> invisibleObjects;
	int nInvisibleObjects;


	//vector<std::map<string,string>>* objects; branca enemics
	std::map<int,Enemy*> enemies;		//(id_enemic,enemic)
	int enemyToErase = -1, eraseAnimationTime;

	int playerScore, timeLeft;
};


#endif // _TILE_MAP_INCLUDE


