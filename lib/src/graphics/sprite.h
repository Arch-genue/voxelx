#pragma once

#include <glm/glm.hpp>
#include "uvregion.h"

class Texture;

class Sprite {
public:
	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 origin;
	glm::vec4 color;
	float angle;
	bool flippedX = false;
	bool flippedY = false;
	Texture* texture;
	UVRegion region;

	Sprite(glm::vec2 position, glm::vec2 size, Texture* texture);
	virtual ~Sprite();

	void setTexture(Texture* texture) {
		this->texture = texture;
	}
};