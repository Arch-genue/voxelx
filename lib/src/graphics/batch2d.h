#pragma once

#include <stdlib.h>
#include <glm/glm.hpp>

#include "uvregion.h"

using namespace glm;

class Mesh;
class Texture;
class Sprite;

class Batch2D {
	float* buffer;
	size_t capacity;
	size_t offset;
	Mesh* mesh;
	size_t index;

	Texture* blank;
	Texture* _texture;	

	void vertex(float x, float y,
			float u, float v,
			float r, float g, float b, float a);
	void vertex(vec2 point,
			vec2 uvpoint,
			float r, float g, float b, float a);

public:
	glm::vec4 color;

	Batch2D(size_t capacity);
	~Batch2D();

	void begin();
	void texture(Texture* texture);
	void sprite(float x, float y, float w, float h, int atlasRes, int index, vec4 tint);
	void sprite(Sprite* sprite);
	void blockSprite(float x, float y, float w, float h, int atlasRes, int index[6], vec4 tint);
	void rect(float x, float y,
			float w, float h,
			float ox, float oy,
			float angle, UVRegion region,
			bool flippedX, bool flippedY,
			vec4 tint);

	void rect(float x, float y, float w, float h);
	void rect(float x, float y, float w, float h,
						float u, float v, float tx, float ty,
						float r, float g, float b, float a);
	void render();
};
