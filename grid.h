#ifndef GRID_H
#define GRID_H

#include <GL/glew.h>

#include "shader.h"

class Grid {
public:
	Grid();
	~Grid();

	void Draw();
	void Update();
	void ToggleCell(int x, int y);
private:
	GLuint gridBuffer;
	GLuint gridArray;

	GLuint cellBuffer;
	GLuint cellArray;

	Shader shader;

	int width;
	int height;
	int cellSize;

	bool* grid;
	int gridBuf;

	void DrawCell(int, int);
	int i;
};

#endif