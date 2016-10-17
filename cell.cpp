#include <fstream>
#include <sstream>

#include "cell.h"

using namespace std;

Cell::Cell() {
	ifstream fragFile("cell.frag");
	stringstream frag;
	frag << fragFile.rdbuf();
	shader.SetFragmentShader(frag.str());

	ifstream vertFile("cell.vert");
	stringstream vert;
	vert << vertFile.rdbuf();
	shader.SetVertexShader(vert.str());

	shader.RegisterAttrib("Position");

	shader.Init();

	GLfloat square[4][3] = {
		{0.0,  0.0, 0.0},
		{1.0,  0.0, 0.0},
		{1.0, -1.0, 0.0},
		{0.0, -1.0, 0.0}
	};

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), square, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);
}

Cell::~Cell() {
	shader.Cleanup();
}

void Cell::Draw() {
	if (shader.IsValid()) {
		shader.UseProgram();
		GLint color = shader.GetUniformLocation("Color");
		glUniform4f(color, 1.0, 0.0, 0.0, 1.0);
		
		const GLfloat scaleMat[16] = {
			1/ 1.6, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1 ,0,
			0, 0, 0, 1
		};
		GLint scale = shader.GetUniformLocation("Scale");
		glUniformMatrix4fv(scale, 1, GL_FALSE, scaleMat);

		const GLfloat translateMat[16] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1 ,0,
			0, 0, 0, 1
		};
		GLint translate = shader.GetUniformLocation("Translate");
		glUniformMatrix4fv(translate, 1, GL_FALSE, translateMat);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
}