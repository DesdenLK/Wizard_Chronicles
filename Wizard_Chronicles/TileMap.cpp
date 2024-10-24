#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include "DynamicObjectBox.h"
#include "DynamicObject.h"


using namespace std;
using json = nlohmann::json;



TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
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
	if (staticObjects != NULL)
		delete staticObjects;
	if (dynamicObjects.size() > 0) {
		for (int i = 0; i < nDynamicObjects; ++i) {
			delete dynamicObjects[i];
		}
	}
}


void TileMap::render() const
{


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

	for (int i = 0; i < nDynamicObjects; ++i) {
		if (dynamicObjects[i] != nullptr) dynamicObjects[i] ->render();
	}



}

void TileMap::update(int deltaTime)
{
	for (int i = 0; i < nDynamicObjects; ++i) {
		if (dynamicObjects[i] != nullptr) dynamicObjects[i]->update(deltaTime);
	}
}

void TileMap::free()
{
	glDeleteBuffers(1, &vboBackground);
	glDeleteBuffers(1, &vboMiddle);
	glDeleteBuffers(1, &vboForeground);
}

void TileMap::initDynamicObjects(const nlohmann::json& j, ShaderProgram &program)
{
	for (int i = 0; i < nDynamicObjects; ++i) {
		auto obj = j[i];
		//cout << obj["type"] << endl;
		
		
		cout << obj["type"] << endl;
		if (obj["type"] == "Box") {
			DynamicObjectBox *box = new DynamicObjectBox();
			box -> init(i,"images/DynamicObjects/Box.png", float(obj["x"]),  float(obj["y"]), float(obj["width"]), float(obj["height"]), glm::vec2(int(obj["width"]), int(obj["height"])), 0.2, 1, glm::vec2(0, 0), program, this);

			//cout << int(obj["width"]) << " " << int(obj["height"]) << endl;

			dynamicObjects[i] = box;
			//cout << "BoxIF" << endl;
		}

		else if (obj["type"] == "Barrel") {
			dynamicObjects[i] = new DynamicObject();
			dynamicObjects[i] -> init(i, "images/DynamicObjects/Barrel.png", float(obj["x"]), float(obj["y"]), float(obj["width"]), float(obj["height"]), glm::vec2(float(obj["width"]), float(obj["height"])), 1, 1, glm::vec2(0,0), program, this);

		}
	}
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

	auto static_objectsJSON = mapFile["layers"][3]["objects"];
	auto dynamic_objectsJSON = mapFile["layers"][4]["objects"];
	
	map = new int[mapSize.x * mapSize.y];
	background = new int[mapSize.x * mapSize.y];
	middle = new int[mapSize.x * mapSize.y];
	foreground = new int[mapSize.x * mapSize.y];
	
	nStaticObjects = static_objectsJSON.size();
	staticObjects = new StaticObject[nStaticObjects];


	copy(background_json.begin(), background_json.end(), background);
	copy(middle_json.begin(), middle_json.end(), middle);
	copy(foreground_json.begin(), foreground_json.end(), foreground);
	// carregar els objectes a this.objects
	for (int i = 0; i < nStaticObjects; ++i) {
		auto obj = static_objectsJSON[i];
		staticObjects[i] = StaticObject(i, obj["type"], float(obj["x"]), float(obj["y"]), float(obj["width"]), float(obj["height"]));
	}

	nDynamicObjects = dynamic_objectsJSON.size();
	dynamicObjects = vector<DynamicObject*>(nDynamicObjects);

	initDynamicObjects(dynamic_objectsJSON, program);



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

	for (int i = 0; i < nStaticObjects; ++i)
	{
		if (staticObjects[i].getType() != "Stair" && staticObjects[i].rightCollision(pos, size))
		{
			return true;
		}

	}

	for (int i = 0; i < nDynamicObjects; ++i)
	{
		if (dynamicObjects[i] != nullptr and dynamicObjects[i] -> IsHitboxEnabled() && dynamicObjects[i]->rightCollision(pos, size))
		{
			return true;
		}

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


	for (int i = 0; i < nStaticObjects; ++i)
	{
		if (staticObjects[i].getType() != "Stair" && staticObjects[i].leftCollision(pos, size))
		{
			return true;
		}

	}

	for (int i = 0; i < nDynamicObjects; ++i)
	{
		if (dynamicObjects[i] != nullptr and dynamicObjects[i]->IsHitboxEnabled() && dynamicObjects[i] -> leftCollision(pos, size))
		{
			return true;
		}

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

	//COL·LISIÓ AMB OBJECTES ESTÀTICS
	for (int i = 0; i < nStaticObjects; ++i)
	{
		if (staticObjects[i].getType() != "Stair" && staticObjects[i].topCollision(pos, size))
		{
			*posY = staticObjects[i].getPosicio().y  - size.y;
			return true;
		}

	}


	for (int i = 0; i < nDynamicObjects; ++i)
	{
		if (dynamicObjects[i] != nullptr and dynamicObjects[i]->IsHitboxEnabled() && dynamicObjects[i] ->topCollision(pos, size))
		{
			*posY = dynamicObjects[i]->getPosicio().y - size.y;
			return true;
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
			if (*posY - tileSize * y - size.y <= 200000)
			{
				*posY = tileSize * y + size.y;
				return true;
			}
		}
	}

	//COL·LISIÓ AMB OBJECTES ESTÀTICS
	for (int i = 0; i < nStaticObjects; ++i)
	{
		if (staticObjects[i].getType() != "Stair" && staticObjects[i].bottomCollision(pos, size))
		{
			*posY = staticObjects[i].getPosicio().y + size.y + 2;
			return true;
		}

	}

	for (int i = 0; i < nDynamicObjects; ++i)
	{
		if (dynamicObjects[i] != nullptr and dynamicObjects[i]->IsHitboxEnabled() && dynamicObjects[i]->bottomCollision(pos, size))
		{
			*posY = dynamicObjects[i]->getPosicio().y + size.y + 2;
			return true;
		}

	}
	

	return false;
}




bool TileMap::ladderCollision(const glm::vec2& pos, const glm::vec2& size)
{
	for (int i = 0; i < nStaticObjects; ++i) {
		if (staticObjects[i].getType() == "Stair") {
			if (staticObjects[i].objectCollision(pos, size)) {
				return true;
			}
		}

	}
	return false;
}

int TileMap::pickingObject(const glm::vec2& pos, const glm::vec2& size)
{
	for (int i = 0; i < nDynamicObjects; ++i) {
		if (dynamicObjects[i] != nullptr) {
			if (dynamicObjects[i]->rightCollision(pos - glm::vec2(4, 0), size) || dynamicObjects[i]->leftCollision(pos + glm::vec2(4, 0), size)) {
				return i;
			}
		}
	}
	return -1;
}

DynamicObject* TileMap::getDynamicObject(int index)
{
	return dynamicObjects[index];
}

void TileMap::destroyDynamicObject(int index)
{
	//delete dynamicObjects[index];
	dynamicObjects[index] = nullptr;
}


bool TileMap::isOnLadderTop(const glm::vec2& posPlayer, const glm::vec2& playerSize) {
	StaticObject ladderObj = staticObjects[0];
	return posPlayer.y <= ladderObj.getPosicio().y;
}

bool TileMap::isOnLadderBottom(const glm::vec2& posPlayer, const glm::vec2& playerSize) {
	StaticObject ladderObj = staticObjects[0];
	return posPlayer.y + playerSize.y >= ladderObj.getPosicio().y + ladderObj.getMeasures().y;
}

void TileMap::renderDynamicObjects()
{
	for (int i = 0; i < nDynamicObjects; ++i) {
		if (dynamicObjects[i] != nullptr) dynamicObjects[i]->render();
	}
}






























