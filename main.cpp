#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "game.h"

int Init();

GLfloat square[4][3] = {
	{0.0,  0.0, 0.0},
	{1.0,  0.0, 0.0},
	{1.0, -1.0, 0.0},
	{0.0, -1.0, 0.0}
};

GLuint vbo;
GLuint vao;

int main(int argc, char** argv) 
{
	if (Init() < 0)
	{
		printf("Could not initialize");
		return -1;
	}

	SDL_Window* window = SDL_CreateWindow(
		"Game Of Life",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1000,
		1000,
		SDL_WINDOW_OPENGL
	);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();

	Game game;

	glClearColor(0.4, 0.4, 0.4, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	SDL_GL_SwapWindow(window);

	while (game.Loop()) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					game.Quit();
			}
		}
		game.Update();
		game.Render();

		SDL_GL_SwapWindow(window);
	}
}

int Init() {
	return SDL_Init(SDL_INIT_VIDEO);
}