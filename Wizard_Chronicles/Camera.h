#ifndef _CAMERA_INCLUDE
#define _CAMERA_INCLUDE

class Camera
{

public:
	glm::mat4 init(glm::vec2 pos, int width, int height);
	glm::mat4 cameraPositionNOCENTRAT(glm::vec2 posPlayer);
	glm::mat4 cameraPositionCENTRAT(glm::vec2 posPlayer, glm::vec2 PlayerV);
	glm::mat4 cameraPositionMix(glm::vec2 posPlayer, glm::vec2 PlayerV);

private:
	glm::mat4 projection;
	glm::vec2 cameraPos;

	int CAMERA_WIDTH;
	int CAMERA_HEIGHT;

};


#endif // _CAMERA_INCLUDE
