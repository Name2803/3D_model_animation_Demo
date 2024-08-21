#pragma once 

#include <stdlib.h>
#include <glad/glad.h>
#include <iostream>

class Mesh {
	GLuint VAO, VBO;
	size_t vertices;
	size_t vertexSize;
	const int* attrs;
public:
	Mesh(const float* buffer, size_t vertices, const int* attrs);
	~Mesh();

	void reload(const float* buffer, size_t vertices);
	void draw(unsigned int primitive);

};