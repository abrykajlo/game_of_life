#ifndef GAME_H
#define GAME_H

#include "grid.h"

class Game {
public:
	Game();
	~Game();

	bool Loop();
	void Quit();
	void Update();
	void TogglePause();
	void ToggleCell(int, int);
	bool IsPaused();
	void Render();
private:
	bool loop;
	Grid grid;
	bool paused;
};

#endif