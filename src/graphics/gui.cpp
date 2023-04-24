#include "gui.h"
#include "mesh.h"
#include "batch2d.h"
#include "shader.h"
#include "../gamesystems/camera.h"
#include "../window/window.h"
#include "../window/input.h"

GUI::GUI() {
	float vertices[] = {
        //x     y
        0.0f,-0.02f,
        0.0f, 0.02f,

        -0.015f, 0.0f,
        0.015f,0.0f,
    };
	int attrs[] = {
		2,  0 //null terminator
	};
	crosshair = new Mesh(vertices, 4, attrs);

	batch = new Batch2D(1024);
	uicamera = new Camera(glm::vec3(), Window::height / 1.0f);
	uicamera->perspective = false;
	uicamera->flipped = true;
}
void GUI::setRenderer(Renderer* render) {
	gui = render;
}

GUI::~GUI() {
	delete crosshair;
	delete batch;
	delete uicamera;
}

void GUI::draw(Texture* text) {
	uicamera->fov = Window::height;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	Shader* uishader = gui->getUIShader();
	uishader->use();
	uishader->uniformMatrix("u_projview", uicamera->getProjection() * uicamera->getView());

	Texture* blocks = nullptr;
	Texture* sprite = text;

	if (!Input::_cursor_locked) {
		batch->texture(nullptr);
		batch->color = vec4(0.0f, 0.0f, 0.0f, 0.5f);
		batch->rect(0, 0, Window::width, Window::height);
	}

	batch->color = vec4(1.0f);
	batch->texture(sprite);
	batch->sprite(16, 16, 64, 64, 16, 0, vec4(1.0f));
	batch->render();

	Shader* crosshairShader = gui->getCrosshairShader();
	crosshairShader->use();
	crosshairShader->uniformFloat("u_ar", (float)Window::height / (float)Window::width);
	crosshairShader->uniformFloat("u_scale", 1.0f / ((float)Window::height / 1000.0f));
	crosshair->draw(GL_LINES);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void GUI::setFont(TTF_Font* font) {
    defaultFont = font;
}

Texture* GUI::renderText(SDL_Color clr, char* string) {
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(defaultFont, string, clr);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surfaceMessage->w, surfaceMessage->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surfaceMessage->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(surfaceMessage);

    return new Texture(texture, surfaceMessage->w, surfaceMessage->h);
}