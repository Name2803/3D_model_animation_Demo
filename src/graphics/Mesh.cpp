#include "Mesh.h"

Mesh::Mesh(const float* buffer, size_t vertices, const int* attrs) : 
	vertices(vertices),
	attrs(attrs)
{
	
	vertexSize = 0;
	for (int i = 0; attrs[i]; i++) {
		vertexSize += attrs[i];
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize * vertices, buffer, GL_STATIC_DRAW);

	// attributes
	int offset = 0;
	for (int i = 0; attrs[i]; i++) {
		int size = attrs[i];
		glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (GLvoid*)(offset * sizeof(float)));
		glEnableVertexAttribArray(i);
		offset += size;
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}


void Mesh::reload(const float* buffer, size_t vertices) {

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize * vertices, buffer, GL_STATIC_DRAW);
	this->vertices = vertices;
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Mesh::draw(unsigned int primitive) {
	glBindVertexArray(VAO);
	glDrawArrays(primitive, 0, vertices);
	glBindVertexArray(0);
}