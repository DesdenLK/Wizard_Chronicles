#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"

using namespace std;
using json = nlohmann::json;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	this->program = &program;
	loadLevel(levelFile, program);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
	if (background != NULL)
		delete background;
	if (middle != NULL)
		delete middle;
	if (foreground != NULL)
		delete foreground;
	if (objects != NULL)
		delete objects;
}


void TileMap::update(int deltaTime) {
	if (enemyToErase != -1) {
		eraseAnimationTime -= deltaTime;
		if (eraseAnimationTime <= 0) {
			enemies.erase(enemyToErase);
			enemyToErase = -1;
		}
	}
	for (auto& enemy : enemies) {
		enemy.second->update(deltaTime);
	}
}


void TileMap::render() const
{
	program->setUniform2f("alpha",1.f,1.f);
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();

	// Render background layer
	glBindVertexArray(vaoBackground);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nBackgroundTiles);

	// Render middle layer
	glBindVertexArray(vaoMiddle);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nMiddleTiles);

	// Render foreground layer
	glBindVertexArray(vaoForeground);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nForegroundTiles);

	glDisable(GL_TEXTURE_2D);

	for (auto& enemy : enemies) {
		enemy.second->render();
	}
}

void TileMap::free()
{
	glDeleteBuffers(1, &vboBackground);
	glDeleteBuffers(1, &vboMiddle);
	glDeleteBuffers(1, &vboForeground);
}

bool TileMap::loadLevel(const string& levelFile, ShaderProgram& program)
{
	ifstream file(levelFile);
	string tileSheetFile;
	json mapFile;

	if (!file.is_open())
	{
		cout << "Error opening file" << endl;
		return false;
	}

	file >> mapFile; //carrega el fitxer json

	mapSize.x = mapFile["width"];
	mapSize.y = mapFile["height"];
	tileSize = mapFile["tilewidth"];
	blockSize = mapFile["tileheight"];
	tileSheetFile = "images/Swamp.png";
	tilesheet.loadFromFile(tileSheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	tilesheetSize.x = 20;
	tilesheetSize.y = 27;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	vector<int> background_json = mapFile["layers"][0]["data"].get<vector<int>>();
	vector<int> middle_json = mapFile["layers"][1]["data"].get<vector<int>>();
	vector<int> foreground_json = mapFile["layers"][2]["data"].get<vector<int>>();
	auto objects_json = mapFile["layers"][3]["objects"];
	auto enemies_json = mapFile["layers"][4]["objects"];
	
	map = new int[mapSize.x * mapSize.y];
	background = new int[mapSize.x * mapSize.y];
	middle = new int[mapSize.x * mapSize.y];
	foreground = new int[mapSize.x * mapSize.y];
	objects = new vector<std::map<string, string>>();
	enemies = std::map<int,Enemy*>();

	copy(background_json.begin(), background_json.end(), background);
	copy(middle_json.begin(), middle_json.end(), middle);
	copy(foreground_json.begin(), foreground_json.end(), foreground);

	// carregar els objectes a this.objects
	for (const auto& obj : objects_json) {
		std::map<string, string> objectMap;
		for (auto it = obj.begin(); it != obj.end(); ++it) {
			objectMap[it.key()] = (string) it.value().dump();
			cout << objectMap[it.key()];
			cout << endl;
		}
		objects->push_back(objectMap);
	}
	// carregar els enemmics a this.enemies
	for (const auto& objEnemy : enemies_json) {
		glm::vec2 enemyStartPos = glm::vec2(0.f, 0.f);
		glm::vec2 enemyBoundingBoxWH = glm::vec2(0.f, 0.f);
		int enemyId = 0;
		for (auto it = objEnemy.begin(); it != objEnemy.end(); ++it) {
			if (it.key() == "x") enemyStartPos[0] = std::stof((string)it.value().dump());
			else if (it.key() == "y") enemyStartPos[1] = std::stof((string)it.value().dump());
			else if (it.key() == "width") enemyBoundingBoxWH[0] = std::stof((string)it.value().dump());
			else if (it.key() == "height") enemyBoundingBoxWH[1] = std::stof((string)it.value().dump());
			else if (it.key() == "id") enemyId = std::stoi((string)it.value().dump());
		}

		for (auto it = objEnemy.begin(); it != objEnemy.end(); ++it) {
			//cout << "key: " << it.key() << ':' << "value: " << (string)it.value().dump() << endl;
			if (it.key() == "type") {
				if ((string)it.value().dump() == "\"Caterpillar\"") {
					CaterpillarEnemy* caterpillar = new CaterpillarEnemy();
					caterpillar->init(enemyId,glm::ivec2(0,16), program, "images/enemics/caterpillar/spriteCaterpillarAmplifiedAlpha.png", glm::ivec2(25, 25), glm::vec2((1.f / 6.f), 1.f), enemyBoundingBoxWH, 1000);
					caterpillar->setPosition(enemyStartPos);
					caterpillar->setTileMap(this);
					cout << "enemy start pos: " << '(' << enemyStartPos[0] << ',' << enemyStartPos[1] << ')' << endl;
					enemies.insert(std::make_pair(enemyId,caterpillar));
				}
				// afegir tipus enemics
			}
		}
		//cout << "nombre d'enemics enregistrats: " << enemies.size() << endl;
	}


	for (int i = 0; i < mapSize.x * mapSize.y; i++)
	{
		map[i] = 0;
		if (background[i] != 0)
		{
			map[i] = background[i];
		}
		if (middle[i] != 0)
		{
			map[i] = middle[i];
		}
		if (foreground[i] != 0)
		{
			map[i] = foreground[i];
		}
	}

	return true;

}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;

	vector<float> backgroundVertices;
	vector<float> middleVertices;
	vector<float> foregroundVertices;

	nBackgroundTiles = 0;
	nMiddleTiles = 0;
	nForegroundTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());

	for (int j = 0; j < mapSize.y; j++) {
		for (int i = 0; i < mapSize.x; i++) {
			posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);

			// Background layer
			tile = background[j * mapSize.x + i];
			if (tile != 0)
			{
				nBackgroundTiles++;
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				texCoordTile[1] -= halfTexel;
				addTileVertices(backgroundVertices, posTile, texCoordTile);
			}

			// Middle layer
			tile = middle[j * mapSize.x + i];
			if (tile != 0)
			{
				nMiddleTiles++;
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				texCoordTile[1] -= halfTexel;
				addTileVertices(middleVertices, posTile, texCoordTile);
			}

			// Foreground layer
			tile = foreground[j * mapSize.x + i];
			if (tile != 0)
			{
				nForegroundTiles++;
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				texCoordTile[1] -= halfTexel;
				addTileVertices(foregroundVertices, posTile, texCoordTile);
			}
		}
	}
	createVAO(vaoBackground, vboBackground, backgroundVertices, program);
	createVAO(vaoMiddle, vboMiddle, middleVertices, program);
	createVAO(vaoForeground, vboForeground, foregroundVertices, program);
}

void TileMap::addTileVertices(vector<float>& vertices, const glm::vec2& posTile, const glm::vec2 texCoordTile[2])
{
	// First triangle
	vertices.push_back(posTile.x); vertices.push_back(posTile.y);
	vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
	vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
	vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
	vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
	vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
	// Second triangle
	vertices.push_back(posTile.x); vertices.push_back(posTile.y);
	vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
	vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
	vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
	vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
	vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
}

void TileMap::createVAO(GLuint& vao, GLuint& vbo, const std::vector<float>& vertices, ShaderProgram& program)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::vec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(middle[y*mapSize.x+x] != 0)
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::vec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(middle[y*mapSize.x+x] != 0)
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveDown(const glm::vec2 &pos, const glm::ivec2 &size, float *posY) const
{
	int x0, x1, y;
	
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		if(middle[y*mapSize.x+x] != 0)
		{
			if(*posY - tileSize * y + size.y <= 200000000)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveUp(const glm::vec2& pos, const glm::ivec2& size, float* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y - size.y + 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (middle[y * mapSize.x + x] != 0)
		{
			if (*posY - tileSize * y - size.y <= 200000000)
			{
				*posY = tileSize * y + size.y;
				return true;
			}
		}
	}

	return false;
}

bool TileMap::boundingBoxCollision(glm::vec2 coordsMin1, glm::vec2 widthHeight1, glm::vec2 coordsMin2, glm::vec2 widthHeight2) {
	// separardes eix x
	if (coordsMin1.x + widthHeight1[0] < coordsMin2.x || coordsMin2.x + widthHeight2[0] < coordsMin1.x)
		return false;

	// separades eix y
	if (coordsMin1.y + widthHeight1[1] < coordsMin2.y || coordsMin2.y + widthHeight2[1] < coordsMin1.y)
		return false;

	return true;
}

bool TileMap::lateralCollisionWithEnemy(const glm::vec2& posPlayer, const glm::vec2& playerSize) {
	for (auto& enemy : enemies) {
		glm::vec2 enemyPos = enemy.second->getPosition();
		glm::vec2 widthHeightEnemyBox = enemy.second->getBoundingBoxWH();
		if (lateralBoxCollision(enemyPos,widthHeightEnemyBox,posPlayer,playerSize)) return true;
	}
	return false;
}

int TileMap::verticalCollisionWithEnemy(const glm::vec2& posPlayer, const glm::vec2& playerSize) {
	for (auto& enemy : enemies) {
		glm::vec2 enemyPos = enemy.second->getPosition();
		glm::vec2 widthHeightEnemyBox = enemy.second->getBoundingBoxWH();
		//cout << "possible collision with Player=(" << posPlayer.x << ',' << posPlayer.y << ",32,32) i Enemic (" << enemyPos.x << ',' << enemyPos.y << ',' << widthHeightEnemyBox[0] << ',' << widthHeightEnemyBox[1] << ')' << endl;
		if (verticalBoxCollision(posPlayer, playerSize, enemyPos, widthHeightEnemyBox)) {
			//cout << "vertical collision with enemy: " << endl;
			return enemy.second->getId();
		}
	}
	return -1;
}

bool TileMap::ladderCollision(const glm::vec2& pos, const glm::vec2& size)
{
	for (const auto& obj : *objects) {
		glm::vec2 objectPos = glm::vec2(std::stof(obj.at("x")), std::stof(obj.at("y")));
		glm::vec2 objectSize = glm::vec2(std::stof(obj.at("width")), std::stof(obj.at("height")));
		/*cout << "Player position checking collision: " << '(' << pos.x << ',' << pos.y << ')' << endl;
		cout << "object position checking collision: " << '(' << objectPos.x << ',' << objectPos.y << ')' << endl;*/
		if (boundingBoxCollision(pos,size,objectPos,objectSize)) {
			//std::cout << "boundingBoxCollision" << endl;
			//std::cout << "object type: " << obj.at("type") << endl;
			if (obj.at("type") == "\"Stair\"") {
				//std::cout << "collided with stair" << endl;
				return true;
			}
		}
	}
	return false;
}

bool TileMap::isOnLadderTop(const glm::vec2& posPlayer, const glm::vec2& PlayerSize) {
	// objects[0] es l escala, si es canvia canviar aquest codi
	std::map<string,string> ladderObj = (*objects)[0];
	return posPlayer.y <= std::stof(ladderObj.at("y"));
}

bool TileMap::isOnLadderBottom(const glm::vec2& posPlayer, const glm::vec2& PlayerSize) {
	// objects[0] es l escala, si es canvia canviar aquest codi
	std::map<string, string> ladderObj = (*objects)[0];
	return posPlayer.y + PlayerSize.y  >= std::stof(ladderObj.at("y")) + std::stof(ladderObj.at("height"));
}

bool TileMap::lateralBoxCollision(glm::vec2 coordsMin1, glm::vec2 widthHeight1, glm::vec2 coordsMin2, glm::vec2 widthHeight2) {
	bool overlapX = (coordsMin1.x < coordsMin2.x + widthHeight2[0]) && (coordsMin1.x + widthHeight1[0] > coordsMin2.x);
	bool overlapY = (coordsMin1.y < coordsMin2.y + widthHeight2[1]) && (coordsMin1.y + widthHeight1[1] > coordsMin2.y);
	return overlapX && overlapY;
}

// true if a vertical collision exists from coordsMin1,widthHeight1 (on top) to coordsMin2,widthHeight2 (on bottom)
bool TileMap::verticalBoxCollision(glm::vec2 coordsMin1, glm::vec2 widthHeight1, glm::vec2 coordsMin2, glm::vec2 widthHeight2) {
	bool overlapX = (coordsMin1.x < coordsMin2.x + widthHeight2[0]) && (coordsMin1.x + widthHeight2[0] > coordsMin2.x);
	bool verticalCollisionFromAbove = (coordsMin1.y + widthHeight1[1] >= coordsMin2.y) && (coordsMin1.y < coordsMin2.y);
	//cout << "overlap en x: " << overlapX << " overlap en y: " << verticalCollisionFromAbove << endl;
	return overlapX && verticalCollisionFromAbove;
}

void TileMap::eraseEnemy(int enemyId) {
	auto& enemy = enemies[enemyId];
	enemy->changeToDeadAnimation();
	enemyToErase = enemyId;
	eraseAnimationTime = enemy->getEraseAnimationTime();
}





























