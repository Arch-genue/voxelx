#include "batch2d.h"
#include "mesh.h"
#include "texture.h"
#include "sprite.h"

#include <GL/glew.h>

#define VERTEX_SIZE 8

Batch2D::Batch2D(size_t capacity) : capacity(capacity), offset(0), color(1.0f, 1.0f, 1.0f, 1.0f){
	int attrs[] = {
			2, 2, 4, 0 //null terminator
	};

	buffer = new float[capacity * VERTEX_SIZE];
	mesh = new Mesh(buffer, 0, attrs);
	index = 0;

	unsigned char pixels[] = {
			255, 255, 255, 255,
	};
	blank = new Texture(pixels, 1, 1);
	_texture = nullptr;
}

Batch2D::~Batch2D(){
	delete blank;
	delete[] buffer;
	delete mesh;
}

void Batch2D::begin(){
	_texture = nullptr;
	blank->bind();
}

void Batch2D::vertex(float x, float y,
		float u, float v,
		float r, float g, float b, float a) {
	buffer[index++] = x;
	buffer[index++] = y;
	buffer[index++] = u;
	buffer[index++] = v;
	buffer[index++] = r;
	buffer[index++] = g;
	buffer[index++] = b;
	buffer[index++] = a;
}
void Batch2D::vertex(vec2 point,
		vec2 uvpoint,
		float r, float g, float b, float a) {
	buffer[index++] = point.x;
	buffer[index++] = point.y;
	buffer[index++] = uvpoint.x;
	buffer[index++] = uvpoint.y;
	buffer[index++] = r;
	buffer[index++] = g;
	buffer[index++] = b;
	buffer[index++] = a;
}

void Batch2D::texture(Texture* new_texture){
	if (_texture == new_texture)
		return;
	render();
	_texture = new_texture;
	if (new_texture == nullptr) {
		blank->bind();
	}else
		new_texture->bind();
}

void Batch2D::rect(float x, float y, float w, float h){
	const float r = color.r;
	const float g = color.g;
	const float b = color.b;
	const float a = color.a;
	if (index + 6*VERTEX_SIZE >= capacity)
		render();

	vertex(x, y, 0, 0, r,g,b,a);
	vertex(x+w, y+h, 1, 1, r,g,b,a);
	vertex(x, y+h, 0, 1, r,g,b,a);

	vertex(x, y, 0, 0, r,g,b,a);
	vertex(x+w, y, 1, 0, r,g,b,a);
	vertex(x+w, y+h, 1, 1, r,g,b,a);
}

void Batch2D::rect(
		float x, float y,
		float w, float h,
		float ox, float oy,
		float angle,
		UVRegion region,
		bool flippedX,
		bool flippedY,
		vec4 tint) {
	if (index + 6*VERTEX_SIZE >= capacity)
		render();

    float centerX = w*ox;
    float centerY = h*oy;
    float acenterX = w-centerX;
    float acenterY = h-centerY;

    float _x1 = -centerX;
    float _y1 = -centerY;

    float _x2 = -centerX;
    float _y2 = +acenterY;

    float _x3 = +acenterX;
    float _y3 = +acenterY;

    float _x4 = +acenterX;
    float _y4 = -centerY;

    float x1,y1,x2,y2,x3,y3,x4,y4;

    if (angle != 0) {
        float s = sin(angle);
        float c = cos(angle);

        x1 = c * _x1 - s * _y1;
        y1 = s * _x1 + c * _y1;

        x2 = c * _x2 - s * _y2;
        y2 = s * _x2 + c * _y2;

        x3 = c * _x3 - s * _y3;
        y3 = s * _x3 + c * _y3;

        x4 = x1 + (x3 - x2);
        y4 = y3 - (y2 - y1);
    } else {
        x1 = _x1;
        y1 = _y1;
        x2 = _x2;
        y2 = _y2;
        x3 = _x3;
        y3 = _y3;
        x4 = _x4;
        y4 = _y4;
    }

    x1 += x; x2 += x; x3 += x; x4 += x;
    y1 += y; y2 += y; y3 += y; y4 += y;

    float u1 = region.u1;
    float v1 = region.v1;
    float u2 = region.u1;
    float v2 = region.v2;
    float u3 = region.u2;
    float v3 = region.v2;
    float u4 = region.u2;
    float v4 = region.v1;

    if (flippedX) {
        float temp = u1;
        u1 = u3;
        u4 = temp;
        u2 = u3;
        u3 = temp;
    }
    if (flippedY) {
        float temp = v1;
        v1 = v2;
        v4 = v2;
        v2 = temp;
        v3 = temp;
    }

    vertex(x1, y1, u1, v1, tint.r, tint.g, tint.b, tint.a);
    vertex(x2, y2, u2, v2, tint.r, tint.g, tint.b, tint.a);
    vertex(x3, y3, u3, v3, tint.r, tint.g, tint.b, tint.a);

    /* Right down triangle */
    vertex(x1, y1, u1, v1, tint.r, tint.g, tint.b, tint.a);
    vertex(x3, y3, u3, v3, tint.r, tint.g, tint.b, tint.a);
    vertex(x4, y4, u4, v4, tint.r, tint.g, tint.b, tint.a);
}

void Batch2D::sprite(Sprite* sprite) {
	vec2 position = sprite->position;
	vec2 size = sprite->size;
	vec2 origin = sprite->origin;
	texture(sprite->texture);
	rect(
		position.x, position.y,
		size.x, size.y,
		origin.x, origin.y,
		sprite->angle,
		sprite->region,
		sprite->flippedX,
		sprite->flippedY,
		sprite->color);
}

void Batch2D::sprite(float x, float y, float w, float h, int atlasRes, int index, vec4 tint) {
	float scale = 1.0f / (float)atlasRes;
	float u = (index % atlasRes) * scale;
	float v = 1.0f - ((index / atlasRes) * scale) - scale;
	rect(x, y, w, h, u, v, scale, scale, tint.r, tint.g, tint.b, tint.a);
}

// void Batch2D::supsprite(float x, float y, float w, float h, int index, vec4 tint){
// 	rect(x, y, w, h, tint.r, tint.g, tint.b, tint.a);
// }

void Batch2D::blockSprite(float x, float y, float w, float h, int atlasRes, int index[6], vec4 tint){
	float scale = 1.0f / (float)atlasRes;
	float uu = (index[3] % atlasRes) * scale;
	float vu = 1.0f - ((index[3] / atlasRes) * scale) - scale;
	float uf = (index[0] % atlasRes) * scale;
	float vf = 1.0f - ((index[0] / atlasRes) * scale) - scale;
	if (this->index + 18*VERTEX_SIZE >= capacity)
		render();

	float ar = 0.88f;
	float ox = x + (w * 0.5f);
	float sx = w * 0.5f * ar;
	vec2 points[7] =   {vec2(ox,        y+(h*0.5f)),
						vec2(ox-sx,     y+(h*0.25f)),
						vec2(ox,        y),
						vec2(ox+sx,     y+(h*0.25f)),
						vec2(ox+sx,     y+(h*0.75f)),
						vec2(ox,        y+h),
						vec2(ox-sx,     y+(h*0.75f))};

	vec2 uvpoints[8] = {vec2(uu,        vu),
						vec2(uu+scale,  vu),
						vec2(uu+scale,  vu+scale),
						vec2(uu,        vu+scale),
						vec2(uf,        vf),
						vec2(uf+scale,  vf),
						vec2(uf+scale,  vf+scale),
						vec2(uf,        vf+scale)};
	
	vertex(points[0], uvpoints[3], tint.r, tint.g, tint.b, tint.a);
	vertex(points[1], uvpoints[0], tint.r, tint.g, tint.b, tint.a);
	vertex(points[2], uvpoints[1], tint.r, tint.g, tint.b, tint.a);

	vertex(points[0], uvpoints[3], tint.r, tint.g, tint.b, tint.a);
	vertex(points[2], uvpoints[1], tint.r, tint.g, tint.b, tint.a);
	vertex(points[3], uvpoints[2], tint.r, tint.g, tint.b, tint.a);

	
	vertex(points[0], uvpoints[7], tint.r, tint.g, tint.b, tint.a);
	vertex(points[3], uvpoints[6], tint.r, tint.g, tint.b, tint.a);
	vertex(points[4], uvpoints[5], tint.r, tint.g, tint.b, tint.a);

	vertex(points[0], uvpoints[7], tint.r, tint.g, tint.b, tint.a);
	vertex(points[4], uvpoints[5], tint.r, tint.g, tint.b, tint.a);
	vertex(points[5], uvpoints[4], tint.r, tint.g, tint.b, tint.a);

	
	vertex(points[0], uvpoints[6], tint.r, tint.g, tint.b, tint.a);
	vertex(points[5], uvpoints[5], tint.r, tint.g, tint.b, tint.a);
	vertex(points[6], uvpoints[4], tint.r, tint.g, tint.b, tint.a);

	vertex(points[0], uvpoints[6], tint.r, tint.g, tint.b, tint.a);
	vertex(points[6], uvpoints[4], tint.r, tint.g, tint.b, tint.a);
	vertex(points[1], uvpoints[7], tint.r, tint.g, tint.b, tint.a);
}

void Batch2D::rect(float x, float y, float w, float h,
					float u, float v, float tx, float ty,
					float r, float g, float b, float a) {
	if (index + 6 * VERTEX_SIZE >= capacity) render();

	vertex(x, y, u, v+ty, r,g,b,a);
	vertex(x+w, y+h, u+tx, v, r,g,b,a);
	vertex(x, y+h, u, v, r,g,b,a);

	vertex(x, y, u, v+ty, r,g,b,a);
	vertex(x+w, y, u+tx, v+ty, r,g,b,a);
	vertex(x+w, y+h, u+tx, v, r,g,b,a);
}

void Batch2D::render() {
	mesh->reload(buffer, index / VERTEX_SIZE);
	mesh->draw(GL_TRIANGLES);
	index = 0;
}