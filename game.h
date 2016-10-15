#ifndef GAME_H
#define GAME_H

#include "cell.h"

class Game {
public:
	Game();
	~Game();

	bool Loop();
	void Quit();
	void Update();
	void Render();
private:
	bool loop;
	Cell cell;
};

#endif