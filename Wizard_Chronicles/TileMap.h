#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <json.hpp>
#include "CaterpillarEnemy.h"
#include <map>


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

class Enemy;

class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::vec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::vec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::vec2 &pos, const glm::ivec2 &size, float *posY) const;
	bool collisionMoveUp(const glm::vec2& pos, const glm::ivec2& size, float* posY) const;
	bool ladderCollision(const glm::vec2& pos, const glm::vec2& size);
	bool isOnLadderTop(const glm::vec2& posPlayer, const glm::vec2& PlayerSize);
	bool isOnLadderBottom(const glm::vec2& posPlayer, const glm::vec2& PlayerSize);
	void update(int deltaTime);
	
private:
	bool loadLevel(const string &levelFile, ShaderProgram& program);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
	void addTileVertices(vector<float>& vertices, const glm::vec2& posTile, const glm::vec2 texCoordTile[2]);
	void createVAO(GLuint& vao, GLuint& vbo, const std::vector<float>& vertices, ShaderProgram& program);
	bool boundingBoxCollision(glm::vec2 coordsMin1, glm::vec2 widthHeight1, glm::vec2 coordsMin2, glm::vec2 widthHeight2);

private:
	GLuint vaoBackground, vaoMiddle, vaoForeground;
	GLuint vboBackground, vboMiddle, vboForeground;
	GLint posLocation, texCoordLocation;
	//ShaderProgram* program;

	int nBackgroundTiles, nMiddleTiles, nForegroundTiles;

	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;

	int* map, * background, * middle, * foreground;
	vector<std::map<string,string>>* objects;
	vector<Enemy*> enemies;
};


#endif // _TILE_MAP_INCLUDE


