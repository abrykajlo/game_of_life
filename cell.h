#ifndef CELL_H
#define CELL_H

#include <GL/glew.h>

#include "shader.h"

class Cell {
public:
	Cell();
	~Cell();

	void Draw();
private:
	Shader shader;
	GLuint vao;
	GLuint vbo;
};

#endif