#include "game.h"

Game::Game()
{
	loop = true;
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

void Game::Update() 
{

}

void Game::Render()
{
	cell.Draw();
}