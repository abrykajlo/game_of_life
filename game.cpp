#include "game.h"

Game::Game()
{
	loop = true;
	paused = false;
}

Game::~Game() 
{
}

bool Game::Loop() 
{
	return loop;
}

void Game::Quit() 
{
	loop = false;
}

bool Game::IsPaused() 
{
	return paused;
}

void Game::TogglePause()
{
	paused = !paused;
}

void Game::Update() 
{
	grid.Update();
}

void Game::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	grid.Draw();
}

void Game::ToggleCell(int x, int y) 
{
	int i = x / 20;
	int j = y / 20;

	grid.ToggleCell(i, j);
}