#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "game.h"

int Init();

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
		1600,
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
					break;
				case SDL_MOUSEBUTTONUP:
					game.ToggleCell(event.button.x, event.button.y);
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym) {
						case SDLK_SPACE:
							game.TogglePause();
					}
			}
		}

		if (game.IsPaused()) {
			game.Update();
		}
		game.Render();

		SDL_GL_SwapWindow(window);

		SDL_Delay(500);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int Init() {
	return SDL_Init(SDL_INIT_VIDEO);
}