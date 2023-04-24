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

void GUI::draw(Texture * text) {
	uicamera->fov = Window::height;

	// glDisable(GL_DEPTH_TEST);
	// glDisable(GL_CULL_FACE);

	Shader* uishader = gui->getUIShader();
	uishader->use();
	uishader->uniformMatrix("u_projview", uicamera->getProjection()*uicamera->getView());

	// Chosen block preview
	Texture* blocks = text;
	//Texture* sprite = assets->getTexture("slot");

	if (!Input::_cursor_locked) {
		batch->texture(nullptr);
		batch->color = vec4(0.0f, 0.0f, 0.0f, 0.5f);
		batch->rect(0, 0, Window::width, Window::height);
	}

	batch->color = vec4(1.0f);
	batch->texture(blocks);
	batch->sprite(16, 16, 64, 64, 16, 0, vec4(1.0f)); // uicamera->fov - 80

	batch->texture(blocks);
	// Player* player = level->player;
	// {
	// 	Block* cblock = Block::blocks[player->choosenBlock];
	// 	if (cblock->model == BLOCK_MODEL_CUBE){
	// 		batch->blockSprite(24, uicamera->fov - 72, 48, 48, 16, cblock->textureFaces, vec4(1.0f));
	// 	} else if (cblock->model == BLOCK_MODEL_GRASS){
	// 		batch->sprite(24, uicamera->fov - 72, 48, 48, 16, cblock->textureFaces[3], vec4(1.0f));
	// 	}
	// }

	if (!Input::_cursor_locked) {
		for (unsigned i = 1; i < 256; i++) {
			//Block* cblock = Block::blocks[i];
			// if (cblock == nullptr)
			// 	break;
			int size = 48;
			int step = 70;
			int x = 24 + (i-1) * step;
			int y = uicamera->fov - 72 - 70;
			y -= 72 * (x / (Window::width - step));
			x %= (Window::width - step);
			vec4 tint(1.0f);
			int mx = Input::x;
			int my = Input::y;
			if (mx > x && mx < x + size && my > y && my < y + size) {
				tint.r *= 2.0f;
				tint.g *= 2.0f;
				tint.b *= 2.0f;
				if (Input::jclicked(SDL_BUTTON_LEFT)) {
					std::cout << "dasdad" << std::endl;
					//player->choosenBlock = i;
				}
			}
			// if (cblock->model == BLOCK_MODEL_CUBE){
			// 	batch->blockSprite(x, y, size, size, 16, cblock->textureFaces, tint);
			// } else if (cblock->model == BLOCK_MODEL_GRASS){
			// 	batch->sprite(x, y, size, size, 16, cblock->textureFaces[3], tint);
			// }
		}
	}

	batch->render();

	Shader* crosshairShader = gui->getCrosshairShader();
	crosshairShader->use();
	crosshairShader->uniformFloat("u_ar", (float)Window::height / (float)Window::width);
	crosshairShader->uniformFloat("u_scale", 1.0f / ((float)Window::height / 1000.0f));
	// crosshairShader->uniform1f("u_scale", 1.0f / ((float)Window::height / 1000.0f));
	crosshair->draw(GL_LINES);
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