#include <fstream>
#include <sstream>

#include "grid.h"

using namespace std;

Grid::Grid() 
{
	ifstream fragFile("GOL.frag");
	stringstream frag;
	frag << fragFile.rdbuf();
	shader.SetFragmentShader(frag.str());

	ifstream vertFile("GOL.vert");
	stringstream vert;
	vert << vertFile.rdbuf();
	shader.SetVertexShader(vert.str());

	shader.RegisterAttrib("Position");

	shader.Init();

	width = 80;
	height = 50;
	cellSize = 20;

	int num_floats = 3 * (2 * (width + 1) + 2 * (height + 1));
	GLfloat vertices[num_floats];
	GLfloat x = 0.0;
	GLfloat y = 0.0;

	int i = 0;
	while (i <= width) {
		int vertex = 6 * i;
		vertices[vertex] = x;
		vertices[vertex + 1] = y;
		vertices[vertex + 2] = 0.0;

		vertices[vertex + 3] = x;
		vertices[vertex + 4] = y + 1000.0;
		vertices[vertex + 5] = 0.0;

		x += 20.0;

		i++;
	}

	x = 0.0;
	y = 0.0;

	int j = 0;
	while (j <= height) {
		int vertex = 6 * i;
		vertices[vertex] = x;
		vertices[vertex + 1] = y;
		vertices[vertex + 2] = 0.0;

		vertices[vertex + 3] = x + 1600.0;
		vertices[vertex + 4] = y;
		vertices[vertex + 5] = 0.0;

		y += 20.0;

		i++;
		j++;
	}

	glGenBuffers(1, &gridBuffer);
	glGenVertexArrays(1, &gridArray);

	glBindVertexArray(gridArray);
	glBindBuffer(GL_ARRAY_BUFFER, gridBuffer);
	glBufferData(GL_ARRAY_BUFFER, num_floats * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);

	GLfloat cell[4][3] = {
		{0.0, 0.0 , 0.0},
		{20.0, 0.0, 0.0},
		{20.0, 20.0, 0.0},
		{0.0, 20.0, 0.0}
	};

	glGenBuffers(1, &cellBuffer);
	glGenVertexArrays(1, &cellArray);

	glBindVertexArray(cellArray);
	glBindBuffer(GL_ARRAY_BUFFER, cellBuffer);

	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), cell, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);

	grid = new bool[width * height * 2];
	for (int i = 0; i < width * height * 2; i++) {
		grid[i] = false;
	}
	gridBuf = 0;
	this->i = 0;
	grid[this->i] = true;
}

Grid::~Grid() 
{
	shader.Cleanup();
	delete [] grid;
}

void Grid::Draw() 
{
	int base = gridBuf * width * height;
	for (int i = 0; i < width * height; i++) {
		if (grid[base + i]) {
			DrawCell(i % width, i / width);
		}
	}

	if (shader.IsValid()) {
		shader.UseProgram();
		GLint color = shader.GetUniformLocation("Color");
		glUniform4f(color, 1.0, 1.0, 1.0, 1.0);
		
		const GLfloat scaleMat[16] = {
			2.0 / 1600.0, 0, 0, 0,
			0, -2.0 / 1000.0, 0, 0,
			0, 0, 1 ,0,
			0, 0, 0, 1
		};
		GLint scale = shader.GetUniformLocation("Scale");
		glUniformMatrix4fv(scale, 1, GL_FALSE, scaleMat);

		const GLfloat translateMat[16] = {
			1, 0, 0, -1.0,
			0, 1, 0, 1.0,
			0, 0, 1 ,0,
			0, 0, 0, 1
		};
		GLint translate = shader.GetUniformLocation("Translate");
		glUniformMatrix4fv(translate, 1, GL_TRUE, translateMat);

		const GLfloat worldTranslateMat[16] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1 ,0,
			0, 0, 0, 1
		};
		GLint worldTranslate = shader.GetUniformLocation("WorldTranslate");
		glUniformMatrix4fv(worldTranslate, 1, GL_TRUE, worldTranslateMat);

		glBindVertexArray(gridArray);
		glDrawArrays(GL_LINES, 0, 2*(width + 1) + 2*(height + 1));
	}
}

void Grid::DrawCell(int x, int y) {
	if (shader.IsValid()) {
		shader.UseProgram();
		GLint color = shader.GetUniformLocation("Color");
		glUniform4f(color, 1.0, 0.0, 0.0, 1.0);
		
		const GLfloat scaleMat[16] = {
			2.0 / 1600.0, 0, 0, 0,
			0, -2.0 / 1000.0, 0, 0,
			0, 0, 1 ,0,
			0, 0, 0, 1
		};
		GLint scale = shader.GetUniformLocation("Scale");
		glUniformMatrix4fv(scale, 1, GL_FALSE, scaleMat);

		const GLfloat translateMat[16] = {
			1, 0, 0, -1.0,
			0, 1, 0, 1.0,
			0, 0, 1 ,0,
			0, 0, 0, 1
		};
		GLint translate = shader.GetUniformLocation("Translate");
		glUniformMatrix4fv(translate, 1, GL_TRUE, translateMat);

		const GLfloat worldTranslateMat[16] = {
			1, 0, 0, 20.0 * x,
			0, 1, 0, 20.0 * y,
			0, 0, 1 ,0,
			0, 0, 0, 1
		};
		GLint worldTranslate = shader.GetUniformLocation("WorldTranslate");
		glUniformMatrix4fv(worldTranslate, 1, GL_TRUE, worldTranslateMat);

		glBindVertexArray(cellArray);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
}

void Grid::Update()
{
	int nextBuf = (gridBuf + 1) % 2;

	int base = gridBuf * width * height;
	int nextBase = nextBuf * width * height;

	for (int i = 0; i < width * height; i++) {
		int num_neighbors = 0;

		int x = i % width;
		int y = i / width;

		//cell indices
		int cell = base + i;
		int top_cell = cell - width;
		int bottom_cell = cell + width;
		int left_cell = cell - 1;
		int right_cell = cell + 1;
		int top_right_cell = top_cell + 1;
		int top_left_cell = top_cell - 1;
		int bottom_right_cell = bottom_cell + 1;
		int bottom_left_cell = bottom_cell - 1;

		//boundaries
		int top = 0;
		int left = 0;
		int right = width - 1;
		int bottom = height - 1;

		if (x != left && grid[left_cell]) {
			num_neighbors++;
		}

		if (x != right && grid[right_cell]) {
			num_neighbors++;
		}

		if (y != top && grid[top_cell]) {
			num_neighbors++;
		}

		if (y != bottom && grid[bottom_cell]) {
			num_neighbors++;
		}

		if ((y != bottom || x != left) && grid[bottom_left_cell]) {
			num_neighbors++;
		}

		if ((y != bottom || x != right) && grid[bottom_right_cell]) {
			num_neighbors++;
		}

		if ((y != top || x != left) && grid[top_left_cell]) {
			num_neighbors++;
		}

		if ((y != top || x != right) && grid[top_right_cell]) {
			num_neighbors++;
		}

		if (grid[base + i]) {
			switch (num_neighbors) {
				case 2:
				case 3:
					grid[nextBase + i] = true;
					break;
				default:
					grid[nextBase + i] = false;
			}	
		} else {
			switch (num_neighbors) {
				case 3:
					grid[nextBase + i] = true;
					break;
				default:
					grid[nextBase + i] = false;
			}
		}
	}

	gridBuf++;
	gridBuf %= 2;
}

void Grid::ToggleCell(int x, int y) 
{
	if (x >= width || y >= height) {
		return;
	}

	int base = gridBuf * width * height;

	grid[base + y * width + x] = !grid[base + y * width + x];
}