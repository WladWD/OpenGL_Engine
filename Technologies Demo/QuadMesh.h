#include "MainHeader.h"

#pragma once
namespace SSAO {
class QuadMesh {
public:
	struct Mesh {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec2 tex0;
	};

public:
	QuadMesh();
	~QuadMesh();

	void bindVAO() const;
	void draw() const;

	GLuint getVAO() const;
	void enableVertexAttribArray() const;
private:
	void initializeVertexBuffer();
	void initializeIndexBuffer();
	void initializeVAO();

	GLuint vao;
	GLuint vertexBuffer, indexBuffer;
};
}
