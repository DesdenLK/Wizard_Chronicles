#pragma once
#include "Scene.h"
class Credit : public Scene
{
public:
	Credit();
	~Credit();

	void init() override;
	void update(int deltaTime) override;
	void render() override;

	private:
		Sprite* background;
		Texture spritesheet;

		void setAnimations();
};

