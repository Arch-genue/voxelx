#include "gui.h"
#include "../window/newwindow.h"
#include "../window/newinput.h"
#include "mesh.h"
#include "batch2d.h"
#include "sprite.h"
#include "shader.h"
#include "../gamesystems/camera.h"
#include "../loaders/resourcemanager.h"

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

	defaultFont = TTF_OpenFont("../res/fonts/arial.ttf", 24);

	SDL_Color clr = {100, 0, 255, 255};
	blocks = renderText(clr, "Hello");
}

GUI::~GUI() {
	delete crosshair;
	delete batch;
	delete uicamera;
}

void GUI::draw() {
	uicamera->fov = Window::height;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	ResourceManager::getShader("ui")->use();
	ResourceManager::getShader("ui")->uniformMatrix("u_projview", uicamera->getProjection() * uicamera->getView());

	Texture* sprite = ResourceManager::getTexture("slot");

	if (Window::getPause()) {
		batch->texture(nullptr);
		batch->color = vec4(0.0f, 0.0f, 0.0f, 0.6f);
		batch->rect(0, 0, Window::width, Window::height);
	}

	batch->color = vec4(1.0f);
	batch->texture(sprite);
	
	batch->sprite(16, uicamera->fov - 80, 64, 64, 16, 0, vec4(1.0f));
	int size = 48;
	int step = 54;
	int defposx = (Window::width / 2 - 4 * step);
	int y = uicamera->fov - 56;

	for (unsigned i = 1; i < 10; i++) {
		int x = defposx + (i-1) * step;
		vec4 tint(1.0f);
		if (!Input::_cursor_locked) {
			int mx = Input::x;
			int my = Input::y;
			if (mx > x && mx < x + size && my > y && my < y + size) {
				tint.r *= 2.0f;
				tint.g *= 2.0f;
				tint.b *= 2.0f;
				if (Input::jclicked(SDL_BUTTON_LEFT)) {
					std::cout << "clicked: " << i << std::endl;
				}
			}
		}
		batch->sprite(x, y, size, size, 16, 0, tint);
	}

	// defposx = 16;
	// for (unsigned x1 = 1; x1 < 6; x1++) {
	// 	for (unsigned y1 = 1; y1 < 6; y1++) {
	// 		int x = defposx + (x1-1) * step;
	// 		int y = 16 + (y1 - 1) * step; //uicamera->fov - 300;
	// 		y -= 56 * (x / (Window::width - step));
	// 		x %= (Window::width - step);
	// 		vec4 tint(1.0f);
	// 		if (!Input::_cursor_locked) {
	// 			int mx = Input::x;
	// 			int my = Input::y;
	// 			if (mx > x && mx < x + size && my > y && my < y + size) {
	// 				tint.r *= 2.0f;
	// 				tint.g *= 2.0f;
	// 				tint.b *= 2.0f;
	// 				if (Input::jclicked(SDL_BUTTON_LEFT)) {
	// 					std::cout << "Inventory clicked: " << x1 << " " << y1 << std::endl;
	// 				}
	// 			}
	// 		}
	// 		batch->sprite(x, y, size, size, 16, 0, tint);
	// 	}
	// }

	// batch->texture(blocks);
	// batch->sprite(150, uicamera->fov - 300, 48, 48, 48, 0, vec4(1.0f));
	batch->render();

	ResourceManager::getShader("crosshair")->use();
	ResourceManager::getShader("crosshair")->uniformFloat("u_ar", (float)Window::height / (float)Window::width);
	ResourceManager::getShader("crosshair")->uniformFloat("u_scale", 1.0f / ((float)Window::height / 1000.0f));
	crosshair->draw(GL_LINES);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void GUI::setFont(TTF_Font* font) {
    defaultFont = font;
}

Texture* GUI::renderText(SDL_Color clr, const char* string) {
    SDL_Surface* surface = TTF_RenderText_Blended(defaultFont, string, clr); //TTF_RenderText_Solid(defaultFont, string, clr);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_FreeSurface(surface);
	
    return new Texture(texture, surface->w, surface->h);
}