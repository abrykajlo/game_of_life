#include <fstream>

#include "cell.h"

using namespace std;

Cell::Cell() {
	ifstream fragFile("cell.frag");
	string frag;
	fragFile >> frag;
	shader.SetFragmentShader(frag);

	ifstream vertFile("cell.vert");
	string vert;
	vertFile >> vert;
	shader.SetVertexShader(vert);

	shader.Init();
}

Cell::~Cell() {

}

void Cell::Draw() {
	if (shader.IsValid()) {
		shader.UseProgram();

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
}